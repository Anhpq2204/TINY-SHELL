#include<iostream>
#include<windows.h>
#include<string.h>
#include<psapi.h>
#include<cstdlib>
#include<sysinfoapi.h>
#include<processthreadsapi.h>
#include <tchar.h>

using namespace std;

void PrintProcessNameAndID( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.

    _tprintf( TEXT("%s\t\t\t\t(PID: %u)\n"), szProcessName, processID );

    // Release the handle to the process.

    CloseHandle( hProcess );
}

int list( void )
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
            PrintProcessNameAndID( aProcesses[i] );
        }
    }

    return 0;
}


void help()
{
    cout<<"\n";//
    cout<<"\t\t\t\tWELCOME TO MyShell\n";//
    cout<<"THE FOLLOWING COMMANDS ARE SUPPORTED:\n";//
    cout<<"list   : Print the list of all background process\n";//
    cout<<"kill   : Terminate a background process\n";//
    cout<<"stop   : Stop a background process\n";//
    cout<<"resume : Resume a background process\n";//
    cout<<"help   : Print this help\n";//
    cout<<"exit   : Close MyShell\n";//
    cout<<"date   : Show or change the current date\n";//
    cout<<"time   : Show or change the current time\n";//
    cout<<"dir    : Print the list of all files and folders existing\n";
    cout<<"path   : Show the current path\n";
    cout<<"addpath: Add a new path\n";
}
void Kill(DWORD  pid)
{
        HANDLE hProcess;
        hProcess= OpenProcess(PROCESS_ALL_ACCESS, FALSE,pid);
        if(hProcess!=NULL)
        {
            TerminateProcess(hProcess,0);
            if(!TerminateProcess(hProcess,0))
            {
                cerr<<"Kill failed\n";
            }
        }
        else{
            cerr<<"Kill failed\n";
        }
        CloseHandle(hProcess);
}
void resume(DWORD pid)
{
    HANDLE hThread;
    hThread=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
    if(hThread!=NULL)
    {
        ResumeThread(hThread);
        if(ResumeThread(hThread)==-1)
        {
            cerr<<"resume failed";
        }
    }
    else{
        cerr<<"resume failed";
    }
}
void createprocess( int argc, TCHAR *argv[] )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 2 )
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process.
    if( !CreateProcess( NULL,   // No module name (use command line)
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    )
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );}
int main()
{   bool k=true;
    while(k)
    {
        cout<<"myShell\\>";
        string s;
        getline(cin,s);
        if(s=="list")
        {   cout<<"PName:\t\t\t\t\tPID:\n";
            list();
        }
        if(s=="help")
        {help();}
        if(s=="exit")
        {
            exit(0);
        }
        if(s=="time")
        {
            SYSTEMTIME stime;
            GetSystemTime(&stime);
            cout<<"The system time is: "<<stime.wHour<<":"<<stime.wMinute<<":"<<stime.wSecond<<"\n";
        }
        if(s=="date")
        {
            SYSTEMTIME stime;
            GetSystemTime(&stime);
            cout<<"The current date is:"<<stime.wDayOfWeek<<" "<<stime.wDay<<"."<<stime.wMonth<<"."<<stime.wYear<<"\n";

        }
        if(s=="kill")
        {
            DWORD  pid;
            cin>>pid;
            Kill(pid);
        }
        if(s=="stop")
        {
            DWORD  pid;
            cin>>pid;
            Kill(pid);
        }
        if(s=="resume")
        {   DWORD pid;
            cin>>pid;
            resume(pid);
        }
    }
}
