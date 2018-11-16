#include "apr_strings.h"
#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include "http_connection.h"

#define OUT_FILTERNAME "TEST_OUT"

#define TEST_ID_LEN 50
#define TEST_KEY_LEN 50

module AP_MODULE_DECLARE_DATA test_module;

typedef struct
{
    char test_id[TEST_ID_LEN + 1];
    char test_key[TEST_KEY_LEN + 1];
} test_key_conf_t;

typedef struct
{
    apr_hash_t *links;
} test_conf;

static int outputFilter( ap_filter_t *f, apr_bucket_brigade *bb )
{
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                   "outputFilter: c:0x%lx",
                   ( long int )f->c );

    // get testId and testKey from configuration
    ap_conf_vector_t *sconf = f->c->base_server->module_config;
    test_conf *conf = ap_get_module_config( sconf, &test_module );

    apr_hash_index_t *hi;
    for ( hi = apr_hash_first( f->c->pool, conf->links ); hi; hi = apr_hash_next( hi ) )
    {
        const void *key;
        void *value;
        apr_hash_this( hi, &key, NULL, &value );
        ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c, "outputFilter ==> key:%s, testid:%s, testkey:%s",
                       ( const char * ) key,
                       ( ( const test_key_conf_t * ) value )->test_id,
                       ( ( const test_key_conf_t * ) value )->test_key );
    }

    // Extract the request line
    apr_bucket *b = APR_BRIGADE_FIRST( bb );
    const char *rqBufer = "";
    apr_size_t rqBytes = 0;
    if ( b != APR_BRIGADE_SENTINEL( bb ) && !( APR_BUCKET_IS_METADATA( b ) ) )
    {
        apr_status_t rv = apr_bucket_read( b, &rqBufer, &rqBytes, APR_BLOCK_READ );
        if ( rv != APR_SUCCESS )
        {
            rqBufer = "";
            rqBytes = 0;
        }
        ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                       "outputFilter: request (%li) \"%.*s\"", rqBytes, ( int ) rqBytes, rqBufer );
    }

    // Find header fields and marker that ends header
    const char *hostBufer = "";
    apr_size_t hostBytes = 0;
    apr_bucket *header_end_bucket = NULL;
    apr_bucket *header_test_bucket = NULL;

    for ( ; b != APR_BRIGADE_SENTINEL( bb ); b = APR_BUCKET_NEXT( b ) )
    {
        if ( !( APR_BUCKET_IS_METADATA( b ) ) )
        {
            const char *buf;
            apr_size_t nbytes;
            apr_status_t rv = apr_bucket_read( b, &buf, &nbytes, APR_BLOCK_READ );
            // Host
            int len = strlen( "TestHeader:" );
            if ( nbytes >= len && !strncmp( "TestHeader:", buf, len ) )
            {
                hostBufer = buf;
                hostBytes = nbytes;
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "outputFilter: TestHeader (%li) \"%.*s\"", hostBytes, ( int ) hostBytes, hostBufer );
                header_test_bucket = b;

                char *header = apr_psprintf( f->c->pool, "%.*s", ( int ) hostBytes, hostBufer );
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "TestHeader: %s", header );

                char *value = "";
                char *strtok_state;

                value = apr_strtok( header, " ", &strtok_state );
                value = apr_strtok( NULL, " ", &strtok_state );
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "TestHeader after apr_strtok(): \"%s\"", value );

                value = apr_strtok( value, "\r\n", &strtok_state );
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "TestHeader after last apr_strtok(): \"%s\"", value );

            }
            else
            {
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "outputFilter: other (%li) \"%.*s\"", nbytes, ( int ) nbytes, buf );
            }
            // End of header is recognized by empty line.
            if ( nbytes == strlen( "\r\n" ) && !strcmp( "\r\n", buf ) )
            {
                header_end_bucket = b;
                break;
            }
        }
    }
    if ( header_end_bucket == NULL ||
         rqBytes == 0 ||
         hostBytes == 0 )
    {
        ap_log_cerror( APLOG_MARK, APLOG_ERR, 0, f->c,
                       "outputFilter: requred header data not found, can not set authorization." );
    }

    if ( header_test_bucket != NULL )
    {
        APR_BUCKET_REMOVE( header_test_bucket );
        ap_log_cerror( APLOG_MARK, APLOG_ERR, 0, f->c,
                       "outputFilter: remove TestHeader" );
    }

    // Pass processing to next filters
    return ap_pass_brigade( f->next, bb );
}

static int test_pre_conn( conn_rec *c, void *csd )
{
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "ap_hook_pre_connection: cip:%s, c:0x%lx",
                   c->client_ip,
                   ( long int )c );

    if ( c->sbh == 0 )
    {
        ap_add_output_filter( OUT_FILTERNAME, NULL, NULL, c );
    }

    return OK;
}

static int test_post_read_request( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "ap_hook_post_read_request: cip:%s, uri:%s, c:0x%lx, r:0x%lx",
                   c->client_ip,
                   r->uri,
                   ( long int )c,
                   ( long int )r );

    char *uri = apr_pcalloc( c->pool, 256 );
    strncpy( uri, r->uri, 256 );
    ap_set_module_config( c->conn_config, &test_module, ( void * )uri );

    return OK;
}

static int my_first_fixup( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "my_first_fixup: uri:%s, r:0x%lx ",
                   r->uri,
                   ( long int )r );

    apr_table_addn( r->headers_in, "TestHeader", r->uri );

    return OK;
}

static int my_last_fixup( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "my_last_fixup: uri:%s, r:0x%lx ",
                   r->uri,
                   ( long int )r );

    const char *header = apr_table_get( r->headers_in, "TestHeader" );

    if ( header )
    {
        ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                       "my_last_fixup: header:%s ",
                       header );

        char *value = apr_pstrdup( r->pool, header );
        char *strtok_state;
        value = apr_strtok( value, "/", &strtok_state );

        ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                       "my_last_fixup: header after strtok:%s ",
                       value );
    }

    //do nothing...
    return OK;
}

static void test_register_hooks( apr_pool_t *p )
{
    ap_register_output_filter( OUT_FILTERNAME, outputFilter, NULL, AP_FTYPE_CONNECTION ) ;

    ap_hook_fixups( my_first_fixup, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_fixups( my_last_fixup, NULL, NULL, APR_HOOK_LAST );

    ap_hook_pre_connection( test_pre_conn, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_post_read_request( test_post_read_request, NULL, NULL, APR_HOOK_FIRST );
}

static void *test_config( apr_pool_t *pool, server_rec *s )
{
    test_conf *ret = apr_pcalloc( pool, sizeof( test_conf ) );

    /* don't initialize links and events until they get set/used */
    return ret;
}

static const char *store_test_id( cmd_parms *cmd, void *CFG, const char *key, const char *testId )
{
    server_rec *s = cmd->server;
    test_key_conf_t *cfg;
    test_conf *conf = ap_get_module_config( s->module_config, &test_module );

    if ( !conf )
    {
        ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                       "store_test_id: cannot find server configuration" );
        return NULL;
    }

    if ( strlen( testId ) > TEST_ID_LEN )
    {
        ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                       "store_test_id: val parameter length should be (0, %i], is %lu.",
                       TEST_ID_LEN, strlen( testId ) );
        return NULL;
    }

    if ( conf->links == NULL )
    {
        conf->links = apr_hash_make( cmd->pool );

        if ( !conf->links )
        {
            ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                           "store_test_id: cannot create links" );
            return NULL;
        }
    }

    cfg = apr_hash_get( conf->links, key, APR_HASH_KEY_STRING );

    if ( !cfg )
    {
        cfg = apr_pcalloc( cmd->pool, sizeof( test_key_conf_t ) );
        strncpy( cfg->test_id, testId,  sizeof( cfg->test_id ) );

        ap_log_perror( APLOG_MARK, APLOG_WARNING, 0, cmd->pool, "store_test_id: key: %s, val: %s",
                       key,
                       cfg->test_id );

        apr_hash_set( conf->links, key, APR_HASH_KEY_STRING, cfg );
    }
    else
    {
        strncpy( cfg->test_id, testId,  sizeof( cfg->test_id ) );

        ap_log_perror( APLOG_MARK, APLOG_WARNING, 0, cmd->pool, "store_test_id: key: %s, val: %s",
                       key,
                       cfg->test_id );
    }

    return NULL;
}

static const char *store_test_key( cmd_parms *cmd, void *CFG, const char *key, const char *testKey )
{
    server_rec *s = cmd->server;
    test_key_conf_t *cfg;
    test_conf *conf = ap_get_module_config( s->module_config, &test_module );

    if ( !conf )
    {
        ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                       "store_test_key: cannot find server configuration" );
        return NULL;
    }

    if ( strlen( testKey ) > TEST_KEY_LEN )
    {
        ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                       "store_test_id: val parameter length should be (0, %i], is %lu.",
                       TEST_KEY_LEN, strlen( testKey ) );
        return NULL;
    }

    if ( conf->links == NULL )
    {
        conf->links = apr_hash_make( cmd->pool );

        if ( !conf->links )
        {
            ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool,
                           "store_test_key: cannot create links" );
            return NULL;
        }
    }

    cfg = apr_hash_get( conf->links, key, APR_HASH_KEY_STRING );

    if ( !cfg )
    {
        cfg = apr_pcalloc( cmd->pool, sizeof( test_key_conf_t ) );
        strncpy( cfg->test_key, testKey,  sizeof( cfg->test_key ) );

        ap_log_perror( APLOG_MARK, APLOG_WARNING, 0, cmd->pool, "store_test_key: key: %s, val: %s",
                       key,
                       cfg->test_key );

        apr_hash_set( conf->links, key, APR_HASH_KEY_STRING, cfg );
    }
    else
    {
        strncpy( cfg->test_key, testKey,  sizeof( cfg->test_key ) );

        ap_log_perror( APLOG_MARK, APLOG_WARNING, 0, cmd->pool, "store_test_key: key: %s, val: %s",
                       key,
                       cfg->test_key );
    }

    return NULL;
}

static const command_rec test_cmds[] =
{
    AP_INIT_ITERATE2( "testId", store_test_id, NULL, OR_FILEINFO, "test Id" ),
    AP_INIT_ITERATE2( "testKey", store_test_key, NULL, OR_FILEINFO, "test Key" ),
    { NULL }
};

AP_DECLARE_MODULE( test ) =
{
    STANDARD20_MODULE_STUFF,
    NULL,                   /* create per-dir    config structures */
    NULL,                   /* merge  per-dir    config structures */
    test_config,            /* create per-server config structures */
    NULL,                   /* merge  per-server config structures */
    test_cmds,              /* table of config file commands       */
    test_register_hooks     /* register hooks                      */
};

//curl -k "http://127.0.0.1:8081/google/imghp"
//curl -k "http://127.0.0.1:8081/yahoo/imghp"
