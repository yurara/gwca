#include "Communicator.h"

GWCAServer* myGWCAServer = new GWCAServer;

class CompletionPort : public AutoHandle
{
public:

    CompletionPort() :
        m_closeHandle(true)
    {
        // Do nothing
    }

    explicit CompletionPort(__in bool closeHandle) :
        m_closeHandle(closeHandle)
    {
        // Do nothing
    }

    CompletionPort(__in bool closeHandle,
                   __in_opt HANDLE handle) :
        m_closeHandle(closeHandle),
        AutoHandle(handle)
    {
        // Do nothing
    }

    ~CompletionPort()
    {
        if (!m_closeHandle)
        {
            Detach();
        }
    }

    __checkReturn HRESULT Create(__in DWORD threadCount)
    {
        Attach(::CreateIoCompletionPort(INVALID_HANDLE_VALUE,
                                        0, // no existing port
                                        0, // ignored
                                        threadCount));

        if (0 == m_h)
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }

        return S_OK;
    }

    __checkReturn HRESULT AssociateFile(__in HANDLE file,
                                        __in ULONG_PTR completionKey)
    {
        ASSERT(0 != file && INVALID_HANDLE_VALUE != file);
        ASSERT(0 != m_h);

        if (0 == ::CreateIoCompletionPort(file,
                                          m_h,
                                          completionKey,
                                          0)) // ignored
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }

        return S_OK;
    }

    __checkReturn HRESULT QueuePacket(__in DWORD bytesCopied,
                                      __in ULONG_PTR completionKey,
                                      __in OVERLAPPED* overlapped)
    {
        ASSERT(0 != m_h);

        if (!::PostQueuedCompletionStatus(m_h,
                                          bytesCopied,
                                          completionKey,
                                          overlapped))
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }

        return S_OK;
    }

    __checkReturn HRESULT DequeuePacket(__in DWORD milliseconds,
                                        __out DWORD& bytesCopied,
                                        __out ULONG_PTR& completionKey,
                                        __out OVERLAPPED*& overlapped)
    {
        ASSERT(0 != m_h);

        if (!::GetQueuedCompletionStatus(m_h,
                                         &bytesCopied,
                                         &completionKey,
                                         &overlapped,
                                         milliseconds))
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }

        return S_OK;
    }

private:

    CompletionPort(CompletionPort&);
    CompletionPort& operator=(CompletionPort&);

    bool m_closeHandle;

} port;

bool GWCAServer::ListenConnections()
{
	bool bConnected = false;
	HANDLE hPipe = NULL;

	port.Create(1);

	while (true)
	{
		Sleep(0);
		hPipe = CreateNamedPipe(
			m_cPipeName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUFSIZE, BUFSIZE,
			1000, NULL);

		bConnected = (ConnectNamedPipe(hPipe, 0) == 1) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

		if(bConnected){
			port.AssociateFile(hPipe, (ULONG_PTR)hPipe);
			LPPIPEINST pOverlapped = (LPPIPEINST)GlobalAlloc(GPTR, sizeof(PIPEINST));
			ReadFile(hPipe,
				&pOverlapped->bmRequest,
				sizeof(pOverlapped->bmRequest),
				&pOverlapped->cbRead,
				(LPOVERLAPPED)pOverlapped);
		}else{
			CloseHandle(hPipe);
		}
	}
	return true;
}

HRESULT GWCAServer::ListenConnectedPipes()
{
	DWORD dwRead = 0;
	HRESULT ret = S_OK;
	ULONG_PTR completitionKey = 0;
	LPOVERLAPPED pOverlapped = NULL;
	LPPIPEINST pPipeInst = NULL;

	while (true)
	{
		ret = port.DequeuePacket(INFINITE, dwRead, completitionKey, pOverlapped);
		if (ret != S_OK)
		{
			GlobalFree((HGLOBAL)pOverlapped);
			CloseHandle((HANDLE)completitionKey);
			continue;
		}

		pPipeInst = reinterpret_cast<LPPIPEINST>(pOverlapped);

		BaseMessage& inMessage = pPipeInst->bmRequest;
		BaseMessage& outMessage = pPipeInst->bmReply;

		if (inMessage.header == CA_DisconnectPipe)
		{
			GlobalFree((HGLOBAL)pPipeInst);
			CloseHandle((HANDLE)completitionKey);
			continue;
		}
		if (inMessage.header == CA_IsAlive)
		{
			//Do Nothing
		}
		else if(inMessage.type & IS_COMMAND)
		{
			if (inMessage.header > CA_RequestsBegin)
			{
				if (inMessage.header >= CA_RequestsEnd)
				{
					outMessage.header = 1;
					outMessage.wParam.i_Param = -1;
					outMessage.lParam.i_Param = -1;
					outMessage.type = IS_NUMERIC;
					//Write(hPipe, outMessage);
				}
				else
				{
					if (m_HandleRequestsFunction != NULL)
					{
						m_HandleRequestsFunction(inMessage.header, inMessage.wParam, inMessage.lParam);
						outMessage = m_CurrResponse;
					}
					else
					{
						outMessage.header = 1;
						outMessage.wParam.i_Param = -1;
						outMessage.lParam.i_Param = -1;
						outMessage.type = IS_NUMERIC;
					}
				}
			}
			else if(inMessage.header <= CA_RequestsBegin)
			{
				if (inMessage.header <= CA_CommandsBegin)
				{
					outMessage.header = 1;
					outMessage.wParam.i_Param = -1;
					outMessage.lParam.i_Param = -1;
					outMessage.type = IS_NUMERIC;
				}
				else
					AddCommand(inMessage);
			}
		}
		else if(inMessage.type & IS_REQUEST)
		{
			ZeroMemory((PVOID)&outMessage, sizeof(outMessage));
			if (inMessage.header < CA_RequestsBegin)
			{
				if (inMessage.header > CA_CommandsBegin)
					AddCommand(inMessage);
				outMessage.header = 1;
				outMessage.wParam.i_Param = -1;
				outMessage.lParam.i_Param = -1;
				outMessage.type = IS_NUMERIC;
				WriteFile((HANDLE)completitionKey,
					&outMessage,
					sizeof(outMessage),
					&pPipeInst->cbWritten,
					NULL);
			}
			else if(inMessage.header >= CA_RequestsEnd)
			{
				outMessage.header = 1;
				outMessage.wParam.i_Param = -1;
				outMessage.lParam.i_Param = -1;
				outMessage.type = IS_NUMERIC;
				WriteFile((HANDLE)completitionKey,
					&outMessage,
					sizeof(outMessage),
					&pPipeInst->cbWritten,
					NULL);
			}
			else
			{
				if (m_HandleRequestsFunction != NULL)
				{
					m_HandleRequestsFunction(inMessage.header, inMessage.wParam, inMessage.lParam);
					outMessage = m_CurrResponse;
				}
				else
				{
					outMessage.header = 1;
					outMessage.wParam.i_Param = -1;
					outMessage.lParam.i_Param = -1;
					outMessage.type = IS_NUMERIC;
				}
				WriteFile((HANDLE)completitionKey,
					&outMessage,
					sizeof(outMessage),
					&pPipeInst->cbWritten,
					NULL);
			}
		}
		ReadFile((HANDLE)completitionKey,
			&pPipeInst->bmRequest,
			sizeof(pPipeInst->bmRequest),
			&pPipeInst->cbRead,
			pOverlapped);
	}
}