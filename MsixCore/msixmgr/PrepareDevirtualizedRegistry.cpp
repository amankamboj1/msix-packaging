#include <windows.h>

#include <shlobj_core.h>
#include <CommCtrl.h>
#include <experimental/filesystem> // C++-standard header file name

#include "PrepareDevirtualizedRegistry.hpp"
#include "GeneralUtil.hpp"
#include <TraceLoggingProvider.h>
#include "MsixTraceLoggingProvider.hpp"
#include "Constants.hpp"
#include "RegistryDevirtualizer.hpp"

using namespace MsixCoreLib;

const PCWSTR PrepareDevirtualizedRegistry::HandlerName = L"PrepareDevirtualizedRegistry";

HRESULT PrepareDevirtualizedRegistry::ExecuteForAddRequest()
{
    RETURN_IF_FAILED(ExtractRegistry());
    return S_OK;
}

HRESULT PrepareDevirtualizedRegistry::ExtractRegistry()
{
    std::wstring registryFilePath = m_msixRequest->GetPackageDirectoryPath() + registryDatFile;

    std::shared_ptr<RegistryDevirtualizer> registryDevirtualizer;
    RETURN_IF_FAILED(RegistryDevirtualizer::Create(registryFilePath, m_msixRequest, &registryDevirtualizer));
    m_msixRequest->SetRegistryDevirtualizer(registryDevirtualizer);

    return S_OK;
}

HRESULT PrepareDevirtualizedRegistry::ExecuteForRemoveRequest()
{
    RETURN_IF_FAILED(ExtractRegistry());
    return S_OK;
}

HRESULT PrepareDevirtualizedRegistry::CreateHandler(MsixRequest * msixRequest, IPackageHandler ** instance)
{
    std::unique_ptr<PrepareDevirtualizedRegistry > localInstance(new PrepareDevirtualizedRegistry(msixRequest));
    if (localInstance == nullptr)
    {
        return E_OUTOFMEMORY;
    }
    *instance = localInstance.release();

    return S_OK;
}