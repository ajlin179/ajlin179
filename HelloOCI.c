#include <stdio.h>
#include <oci.h>

#define USERNAME "testheraapp"
#define PASSWORD "testpasswd"
#define DATABASE "your_database"

void checkerr(OCIError *err, sword status);

int main() {
    OCIEnv *envhp;      /* OCI environment handle */
    OCIServer *srvhp;   /* OCI server handle */
    OCIError *errhp;    /* OCI error handle */
    OCISession *usrhp;  /* OCI user session handle */
    OCISvcCtx *svchp;   /* OCI service handle */
    sword status;
    text sessionUsername[100];
    OCIStmt *stmthp;    /* OCI statement handle */
    OCIDefine *defhp1, *defhp2;  /* OCI define handles */
    text query[] = "SELECT id, name FROM test";
    text emp_id[50];
    text emp_name[50];

    /* Initialize the OCI environment */
    OCIInitialize(OCI_DEFAULT, 0, 0, 0, 0);

    /* Allocate an environment handle */
    OCIEnvInit(&envhp, OCI_DEFAULT, 0, 0);

    /* Allocate error handle */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&errhp, OCI_HTYPE_ERROR, 0, 0);

    /* Initialize the server handle */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&srvhp, OCI_HTYPE_SERVER, 0, 0);

    /* Initialize the service handle */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&svchp, OCI_HTYPE_SVCCTX, 0, 0);

    /* Create a server context */
    status = OCIServerAttach(srvhp, errhp, (text *)"", strlen(""), OCI_DEFAULT);
    checkerr(errhp, status);

    /* Set server attribute in the service context */
    OCIAttrSet((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp, 0, OCI_ATTR_SERVER, errhp);

    /* Allocate a user session handle */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&usrhp, OCI_HTYPE_SESSION, 0, 0);

    /* Set user session attribute */
    OCIAttrSet((dvoid *)usrhp, OCI_HTYPE_SESSION, (dvoid *)USERNAME, strlen(USERNAME), OCI_ATTR_USERNAME, errhp);
    OCIAttrSet((dvoid *)usrhp, OCI_HTYPE_SESSION, (dvoid *)PASSWORD, strlen(PASSWORD), OCI_ATTR_PASSWORD, errhp);

    /* Begin the session */
    status = OCISessionBegin(svchp, errhp, usrhp, OCI_CRED_RDBMS, OCI_DEFAULT);
    checkerr(errhp, status);

    /* Set the user context attribute in the service context */
    OCIAttrSet((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)usrhp, 0, OCI_ATTR_SESSION, errhp);

    /* Prepare the SQL statement */
    OCIHandleAlloc((dvoid *)envhp, (dvoid **)&stmthp, OCI_HTYPE_STMT, 0, 0);
    OCIStmtPrepare(stmthp, errhp, query, strlen(query), OCI_NTV_SYNTAX, OCI_DEFAULT);

    /* Define the output variables for the result */
    OCIDefineByPos(stmthp, &defhp1, errhp, 1, (dvoid *)emp_id, sizeof(emp_id), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmthp, &defhp2, errhp, 2, (dvoid *)emp_name, sizeof(emp_name), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);

    /* Execute the statement */
    status = OCIStmtExecute(svchp, stmthp, errhp, 1, 0, NULL, NULL, OCI_DEFAULT);
    checkerr(errhp, status);
    printf("\n");
    printf("Employee ID: %s, First Name: %s\n",  emp_id, emp_name);
    /* Fetch and print the result */
    while (OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT) == OCI_SUCCESS) {
        printf("\n");
        printf("Employee ID: %s, First Name: %s\n",  emp_id, emp_name);
    }

    /* End the session */
    OCISessionEnd(svchp, errhp, usrhp, OCI_DEFAULT);

    /* Detach from the server */
    OCIServerDetach(srvhp, errhp, OCI_DEFAULT);

    /* Free handles */
    OCIHandleFree((dvoid *)srvhp, OCI_HTYPE_SERVER);
    OCIHandleFree((dvoid *)usrhp, OCI_HTYPE_SESSION);
    OCIHandleFree((dvoid *)svchp, OCI_HTYPE_SVCCTX);
    OCIHandleFree((dvoid *)stmthp, OCI_HTYPE_STMT);
    OCIHandleFree((dvoid *)defhp1, OCI_HTYPE_DEFINE);
    OCIHandleFree((dvoid *)defhp2, OCI_HTYPE_DEFINE);
    OCIHandleFree((dvoid *)errhp, OCI_HTYPE_ERROR);
    OCIHandleFree((dvoid *)envhp, OCI_HTYPE_ENV);

    return 0;
}

void checkerr(OCIError *err, sword status) {
    text errbuf[512];
    sb4 errcode = 0;

    if (status == OCI_SUCCESS) return;

    switch (status) {
        case OCI_SUCCESS_WITH_INFO:
            printf("OCI_SUCCESS_WITH_INFO\n");
            break;
        case OCI_NEED_DATA:
            printf("OCI_NEED_DATA\n");
            break;
        case OCI_NO_DATA:
            printf("OCI_NO_DATA\n");
            break;
        case OCI_ERROR:
            OCIErrorGet((dvoid *)err, (ub4)1, (text *)NULL, &errcode, errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
            printf("OCI_ERROR: %s\n", errbuf);
            break;
        case OCI_INVALID_HANDLE:
            printf("OCI_INVALID_HANDLE\n");
            break;
        case OCI_STILL_EXECUTING:
            printf("OCI_STILL_EXECUTING\n");
            break;
        case OCI_CONTINUE:
            printf("OCI_CONTINUE\n");
            break;
        default:
            break;
    }
}
