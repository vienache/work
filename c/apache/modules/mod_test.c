#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include "http_connection.h"

#define OUT_FILTERNAME "TEST_OUT"
#define IN_FILTERNAME "TEST_IN"

module AP_MODULE_DECLARE_DATA test_module;

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
        return ap_pass_brigade( f->next, bb );
    }

    // Pass processing to next filters
    return ap_pass_brigade( f->next, bb );
}

static apr_status_t inputFilter( ap_filter_t *f,
                                 apr_bucket_brigade *bb,
                                 ap_input_mode_t mode,
                                 apr_read_type_e block,
                                 apr_off_t readbytes )
{
    apr_status_t status;
    char *uri = ( char * ) ap_get_module_config( f->c->conn_config, &test_module );
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                   "inputFilter: uri:%s, c:0x%lx, r:0x%lx",
                   uri,
                   ( long int )f->c,
                   ( long int )f->r );



    /* just get out of the way of things we don't want. */
    if ( mode != AP_MODE_READBYTES && mode != AP_MODE_GETLINE )
    {
        ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                       "inputFilter: next bb" );
        return ap_get_brigade( f->next, bb, mode, block, readbytes );
    }

    apr_table_setn( f->r->headers_out, "TestHeader", "10" );
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                   "inputFilter: TestHeader:10" );

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
                       "inputFilter: request (%li) \"%.*s\"", rqBytes, ( int ) rqBytes, rqBufer );
    }
    else
    {
        ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                       " APR_BRIGADE_SENTINEL( bb ) is null" );
    }

    // Find header fields and marker that ends header
    const char *hostBufer = "";
    apr_size_t hostBytes = 0;
    apr_bucket *header_end_bucket = NULL;

    for ( ; b != APR_BRIGADE_SENTINEL( bb ); b = APR_BUCKET_NEXT( b ) )
    {
        if ( !( APR_BUCKET_IS_METADATA( b ) ) )
        {
            const char *buf;
            apr_size_t nbytes;
            apr_status_t rv = apr_bucket_read( b, &buf, &nbytes, APR_BLOCK_READ );
            // Host
            int len = strlen( "Test:" );
            if ( nbytes >= len && !strncmp( "Test:", buf, len ) )
            {
                hostBufer = buf;
                hostBytes = nbytes;
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "inputFilter: Test (%li) \"%.*s\"", hostBytes, ( int ) hostBytes, hostBufer );
            }
            else
            {
                ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                               "inputFilter: other (%li) \"%.*s\"", nbytes, ( int ) nbytes, buf );
            }
            // End of header is recognized by empty line.
            if ( nbytes == strlen( "\r\n" ) && !strcmp( "\r\n", buf ) )
            {
                header_end_bucket = b;
                break;
            }
        }
    }

    status = ap_get_brigade( f->next, bb, mode, block, readbytes );

    return status;
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

//static void insert_input_filter(request_rec *r)
//{
//    ap_add_input_filter( IN_FILTERNAME, NULL, r, r->connection );
//}

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

//static void insert_output_filter(request_rec * r)
//{
//    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, r->connection,
//                   "insert_output_filter: c:0x%lx",
//                   r->connection );
//    ap_add_output_filter( OUT_FILTERNAME, NULL, r, r->connection);
//}

static int my_fixup( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "my_fixup: uri:%s, r:0x%lx",
                   r->uri,
                   ( long int )r );

    ap_add_input_filter( IN_FILTERNAME, NULL, r, c );

    return OK;
}


static int test_handler( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "test_handler: r:0x%lx",
                   ( long int )r );
    return OK;
}

static void test_register_hooks( apr_pool_t *p )
{
    // http handling, with body
    ap_register_output_filter( OUT_FILTERNAME, outputFilter, NULL, AP_FTYPE_CONNECTION ) ;

    ap_register_input_filter( IN_FILTERNAME, inputFilter, NULL, AP_FTYPE_CONTENT_SET );

    ap_hook_fixups( my_fixup, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_pre_connection( test_pre_conn, NULL, NULL, APR_HOOK_FIRST );

    //ap_hook_insert_filter( insert_output_filter, NULL, NULL, APR_HOOK_LAST );

    //ap_hook_insert_filter( insert_input_filter, NULL, NULL, APR_HOOK_LAST );

    ap_hook_handler( test_handler, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_post_read_request( test_post_read_request, NULL, NULL, APR_HOOK_FIRST );
}

static const char *store_test_id( cmd_parms *cmd, void *cfg, int argc, char *const argv[] )
{
    ap_log_perror( APLOG_MARK, APLOG_ERR, 0, cmd->pool, "store_test_id" );

    return NULL;
}

static const command_rec test_cmds[] =
{
    AP_INIT_TAKE_ARGV( "testId", store_test_id, NULL, OR_FILEINFO, "test Id" ),
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

//modules/session/mod_session.c +498
//vim modules/filters/mod_substitute.c
