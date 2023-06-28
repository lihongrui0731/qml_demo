#ifndef ADJUSTPRIVILEGE_H
#define ADJUSTPRIVILEGE_H

#include <Windows.h>
#include <minwindef.h>

class AdjustPrivilege
{
    public:
    AdjustPrivilege():m_bEnble(FALSE)
    {
        HANDLE hToken = NULL;
        if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken))
        {
            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
            {
                return;
            }
        }
        LUID luid;
        BOOL bRet = ::LookupPrivilegeValue(NULL,
            SE_SYSTEMTIME_NAME, &luid);
        PRIVILEGE_SET privs;
        privs.PrivilegeCount = 1;
        privs.Control = PRIVILEGE_SET_ALL_NECESSARY;

        privs.Privilege[0].Luid = luid;
        privs.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;

        BOOL bresult = FALSE;
        BOOL bret = ::PrivilegeCheck(hToken, &privs, &bresult);
        if (bresult)
        {
            m_bEnble = TRUE;
        }
        else
        {
            m_bEnble = FALSE;
            SetPrivilege(TRUE, SE_SYSTEMTIME_NAME);
        }

    }
    ~AdjustPrivilege()
    {
        if (m_bEnble == FALSE)
        {
            SetPrivilege(FALSE, SE_SYSTEMTIME_NAME);
        }
    }
private:
    BOOL SetPrivilege(BOOL bSet, LPCWSTR lpszPri)
    {
        HANDLE hTokenProc = NULL;
        BOOL bRet = ::OpenProcessToken( ::GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES, &hTokenProc);
        if (bRet)
        {
            bRet = SetPrivilegeToken(hTokenProc, lpszPri, bSet);
        }
        ::CloseHandle(hTokenProc);

        return bRet;
    }

    BOOL SetPrivilegeToken(HANDLE hToken, LPCWSTR lpszPrivilege, BOOL bEnablePrivilege)
    {
        LUID luid;
        BOOL bRet = ::LookupPrivilegeValue(NULL,
            lpszPrivilege, &luid);
        if (!bRet)
        {
            return FALSE;
        }

        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount       = 1;
        tp.Privileges[0].Luid   = luid;
        tp.Privileges[0].Attributes = bEnablePrivilege?SE_PRIVILEGE_ENABLED:SE_PRIVILEGE_REMOVED;

        return ::AdjustTokenPrivileges( hToken, FALSE, &tp, 0, NULL, NULL );
    }

public:
    BOOL m_bEnble;
};

#endif // ADJUSTPRIVILEGE_H
