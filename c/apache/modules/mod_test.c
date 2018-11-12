#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include "http_connection.h"

#define OUT_FILTERNAME "TEST_OUT"

module AP_MODULE_DECLARE_DATA test_module;

typedef struct
{
    char test_id[100];
    char test_key[100];
} test_conf_t;

typedef struct
{
    int sz;
    test_conf_t arr[4];
} test_conf_arr_t;

static test_conf_t test_conf;

static test_conf_arr_t test_conf_arr_ids;
static test_conf_arr_t test_conf_arr_key;

static int outputFilter( ap_filter_t *f, apr_bucket_brigade *bb )
{
    char *uri = ( char * ) ap_get_module_config( f->c->conn_config, &test_module );
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                   "outputFilter: uri:%s, c:0x%lx, r:0x%lx",
                   uri,
                   ( long int )f->c,
                   ( long int )f->r );

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

static int my_fixup( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "my_fixup: uri:%s, r:0x%lx test_id:%s, test_key:%s",
                   r->uri,
                   ( long int )r,
                   test_conf.test_id,
                   test_conf.test_key );

    apr_table_addn( r->headers_in, "TestHeader", r->uri );

    for (int i=0; i<test_conf_arr_key.sz; ++i)
    {
        ap_log_cerror( APLOG_MARK, APLOG_ERR, 0, c,
                       "my_fixup: test_key[%i]=%s",
                       i,
                       test_conf_arr_key.arr[i].test_key);
    }

    for (int i=0; i<test_conf_arr_ids.sz; ++i)
    {
        ap_log_cerror( APLOG_MARK, APLOG_ERR, 0, c,
                       "my_fixup: test_ids[%i]=%s",
                       i,
                       test_conf_arr_ids.arr[i].test_id);
    }

    return OK;
}

static void test_register_hooks( apr_pool_t *p )
{
    ap_register_output_filter( OUT_FILTERNAME, outputFilter, NULL, AP_FTYPE_CONNECTION ) ;

    ap_hook_fixups( my_fixup, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_pre_connection( test_pre_conn, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_post_read_request( test_post_read_request, NULL, NULL, APR_HOOK_FIRST );
}

static const char *store_test_id( cmd_parms *cmd, void *cfg, int argc, char *const argv[] )
{
    if ( argc != 1 )
    {
        ap_log_perror( APLOG_MARK, APLOG_WARNING, 0, cmd->pool,
                       "testId requires exactly one argument." );
    }
    else
    {
        strcpy( test_conf.test_id, argv[0] );
        ap_log_perror( APLOG_MARK, APLOG_WARNING, 0, cmd->pool, "testId: %s",
                       test_conf.test_id );

        int pos = test_conf_arr_ids.sz;
        strncpy(test_conf_arr_ids.arr[pos].test_id, argv[0], 100);
        test_conf_arr_ids.sz++;
        //TODO: ProxyHTMLLinks
    }

    return NULL;
}

static const char *store_test_key( cmd_parms *cmd, void *dummy, const char *arg )
{
    int pos = test_conf_arr_key.sz;
    strncpy(test_conf_arr_key.arr[pos].test_key, arg, 100);
    test_conf_arr_key.sz++;

    return NULL;
}

static const command_rec test_cmds[] =
{
    AP_INIT_TAKE_ARGV( "testId", store_test_id, NULL, OR_FILEINFO, "test Id" ),
    AP_INIT_ITERATE( "testKey", store_test_key, NULL, RSRC_CONF, "test Key" ),
    { NULL }
};

AP_DECLARE_MODULE( test ) =
{
    STANDARD20_MODULE_STUFF,
    NULL,                   /* create per-dir    config structures */
    NULL,                   /* merge  per-dir    config structures */
    NULL,                   /* create per-server config structures */
    NULL,                   /* merge  per-server config structures */
    test_cmds,           /* table of config file commands       */
    test_register_hooks  /* register hooks                      */
};

//curl -k "http://127.0.0.1:8081/google/imghp"
//curl -k "http://127.0.0.1:8081/yahoo/imghp"
