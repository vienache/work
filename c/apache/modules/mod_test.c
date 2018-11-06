#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include "http_connection.h"

#define OUT_FILTERNAME "TEST_OUT"

module AP_MODULE_DECLARE_DATA test_module;

static int outputFilter( ap_filter_t *f, apr_bucket_brigade *bb )
{
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                   "outputFilter next:%i", ( f->next != 0 ) );
    if ( f->r )
    {
        if ( f->r->uri )
        {
            ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, f->c,
                           "outputFilter: uri:%s, c:0x%lx, r:0x%lx",
                           f->r->uri,
                           ( long int )f->c,
                           ( long int )f->r );
        }
    }

    ap_remove_output_filter( f );

    // Pass processing to next filters
    return ap_pass_brigade( f->next, bb );
}

static int test_pre_conn( conn_rec *c, void *csd )
{
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "test_pre_conn: cip:%s, c:0x%lx",
                   c->client_ip, ( long int )c );

    ap_add_output_filter( OUT_FILTERNAME, NULL, NULL, c );

    return OK;
}

static int test_post_read_request( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "test_post_read_request: cip:%s, uri:%s, c:0x%lx, r:0x%lx",
                   c->client_ip,
                   r->uri,
                   ( long int )c,
                   ( long int )r );

    char *uri = apr_pcalloc( c->pool, 256 );
    strncpy( uri, r->uri, 256 );
    ap_set_module_config( c->conn_config, &test_module, ( void * )uri );

    return OK;
}

static void insert_output_filter( request_rec *r )
{
    conn_rec *c = r->connection;
    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
                   "insert_output_filter: r:0x%lx, c:0x%lx",
                   ( long int )r,
                   ( long int )c );
    ap_add_output_filter( OUT_FILTERNAME, NULL, r, r->connection );
}

//static int include_fixup(request_rec *r)
//{
//    conn_rec *c = r->connection;
//    ap_log_cerror( APLOG_MARK, APLOG_WARNING, 0, c,
//                   "include_fixup: r:0x%lx, c:0x%lx",
//                   ( long int )r,
//                   ( long int )c );
//    ap_add_output_filter( OUT_FILTERNAME, NULL, r, r->connection);
//    return DECLINED;
//}

static void test_register_hooks( apr_pool_t *p )
{
    // http handling, with body
    ap_register_output_filter( OUT_FILTERNAME, outputFilter, NULL, AP_FTYPE_CONNECTION ) ;

    ap_hook_pre_connection( test_pre_conn, NULL, NULL, APR_HOOK_FIRST );

    ap_hook_insert_filter( insert_output_filter, NULL, NULL, APR_HOOK_FIRST );
//    ap_hook_fixups(include_fixup, NULL, NULL, APR_HOOK_LAST);
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
