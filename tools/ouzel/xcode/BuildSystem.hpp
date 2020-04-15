// Copyright 2015-2020 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_XCODE_PROJECT_HPP
#define OUZEL_XCODE_PROJECT_HPP

#include <fstream>
#include "Project.hpp"
#include "PBXBuildFile.hpp"
#include "PBXContainerItemProxy.hpp"
#include "PBXFileReference.hpp"
#include "PBXFrameworksBuildPhase.hpp"
#include "PBXGroup.hpp"
#include "PBXLegacyTarget.hpp"
#include "PBXNativeTarget.hpp"
#include "PBXProject.hpp"
#include "PBXReferenceProxy.hpp"
#include "PBXResourcesBuildPhase.hpp"
#include "PBXShellScriptBuildPhase.hpp"
#include "PBXSourcesBuildPhase.hpp"
#include "PBXTargetDependency.hpp"
#include "XCBuildConfiguration.hpp"
#include "XCConfigurationList.hpp"
#include "storage/FileSystem.hpp"
#include "utils/Plist.hpp"

namespace ouzel
{
    namespace xcode
    {
        class Objects final
        {
        public:
            template <class T, class ...Args>
            T* alloc()
            {
                std::unique_ptr<T> object = std::make_unique<T>();
                T* result = object.get();
                objects.push_back(std::move(object));
                return result;
            }

            const std::vector<std::unique_ptr<PBXObject>>& getObjects() const noexcept
            {
                return objects;
            }

        private:
            std::vector<std::unique_ptr<PBXObject>> objects;
        };

        inline void generateBuildFiles(const Project& project)
        {
            Objects objects;

            const storage::Path projectFilename = project.getPath().getFilename();
            const storage::Path projectDirectory = project.getPath().getDirectory();

            const auto xcodeProjectDirectory = projectDirectory / storage::Path{project.getName() + ".xcodeproj"};
            const auto xcodeProjectDirectoryType = xcodeProjectDirectory.getType();

            if (xcodeProjectDirectoryType == storage::Path::Type::NotFound)
                storage::FileSystem::createDirectory(xcodeProjectDirectory);
            else if (xcodeProjectDirectoryType != storage::Path::Type::Directory)
            {
                storage::FileSystem::deleteFile(xcodeProjectDirectory);
                storage::FileSystem::createDirectory(xcodeProjectDirectory);
            }

            auto pbxProject = objects.alloc<PBXProject>();
            pbxProject->organization = project.getOrganization();

            auto mainGroup = objects.alloc<PBXGroup>();
            mainGroup->sourceTree = PBXSourceTree::Group;
            pbxProject->mainGroup = mainGroup;

            const auto ouzelProjectPath = project.getOuzelPath() / "build" / "ouzel.xcodeproj";
            auto ouzelProjectFileRef = objects.alloc<PBXFileReference>();
            ouzelProjectFileRef->name = "ouzel.xcodeproj";
            ouzelProjectFileRef->path = ouzelProjectPath;
            ouzelProjectFileRef->fileType = PBXFileType::WrapperPBProject;
            ouzelProjectFileRef->sourceTree = PBXSourceTree::Group;
            mainGroup->children.push_back(ouzelProjectFileRef);
            pbxProject->projectReferences["ProjectRef"] = ouzelProjectFileRef;

            auto ouzelPoductRefGroup = objects.alloc<PBXGroup>();
            ouzelPoductRefGroup->name = "Products";
            ouzelPoductRefGroup->path = storage::Path{};
            ouzelPoductRefGroup->sourceTree = PBXSourceTree::Group;
            pbxProject->projectReferences["ProductGroup"] = ouzelPoductRefGroup;

            PBXObject libouzelIos(Id{0x30, 0x3B, 0x75, 0x33, 0x1C, 0x2A, 0x3C, 0x58, 0x00, 0xFE, 0xDE, 0x92});

            auto libouzelIosProxy = objects.alloc<PBXContainerItemProxy>();
            libouzelIosProxy->containerPortal = ouzelProjectFileRef;
            libouzelIosProxy->proxyType = PBXContainerItemProxy::Reference;
            libouzelIosProxy->remoteGlobal = &libouzelIos;
            libouzelIosProxy->remoteInfo = "libouzel_ios";

            auto libouzelIosReferenceProxy = objects.alloc<PBXReferenceProxy>();
            libouzelIosReferenceProxy->path = "libouzel_ios.a";
            libouzelIosReferenceProxy->fileType = PBXFileType::ArchiveAr;
            libouzelIosReferenceProxy->sourceTree = PBXSourceTree::BuildProductsDir;
            libouzelIosReferenceProxy->remoteRef = libouzelIosProxy;
            ouzelPoductRefGroup->children.push_back(libouzelIosReferenceProxy);

            PBXObject libouzelMacOs(Id{0x30, 0x4A, 0x8E, 0x25, 0x1C, 0x23, 0x7C, 0x30, 0x00, 0x8B, 0x11, 0x51});

            auto libouzelMacOsProxy = objects.alloc<PBXContainerItemProxy>();
            libouzelMacOsProxy->containerPortal = ouzelProjectFileRef;
            libouzelMacOsProxy->proxyType = PBXContainerItemProxy::Reference;
            libouzelMacOsProxy->remoteGlobal = &libouzelMacOs;
            libouzelMacOsProxy->remoteInfo = "libouzel_macos";

            auto libouzelMacOsReferenceProxy = objects.alloc<PBXReferenceProxy>();
            libouzelMacOsReferenceProxy->path = "libouzel_macos.a";
            libouzelMacOsReferenceProxy->fileType = PBXFileType::ArchiveAr;
            libouzelMacOsReferenceProxy->sourceTree = PBXSourceTree::BuildProductsDir;
            libouzelMacOsReferenceProxy->remoteRef = libouzelMacOsProxy;
            ouzelPoductRefGroup->children.push_back(libouzelMacOsReferenceProxy);

            PBXObject libouzelTvos(Id{0x30, 0x3B, 0x76, 0x80, 0x1C, 0x35, 0x5A, 0x3B, 0x00, 0xFE, 0xDE, 0x92});

            auto libouzelTvosProxy = objects.alloc<PBXContainerItemProxy>();
            libouzelTvosProxy->containerPortal = ouzelProjectFileRef;
            libouzelTvosProxy->proxyType = PBXContainerItemProxy::Reference;
            libouzelTvosProxy->remoteGlobal = &libouzelTvos;
            libouzelTvosProxy->remoteInfo = "libouzel_tvos";

            auto libouzelTvosReferenceProxy = objects.alloc<PBXReferenceProxy>();
            libouzelTvosReferenceProxy->path = "libouzel_tvos.a";
            libouzelTvosReferenceProxy->fileType = PBXFileType::ArchiveAr;
            libouzelTvosReferenceProxy->sourceTree = PBXSourceTree::BuildProductsDir;
            libouzelTvosReferenceProxy->remoteRef = libouzelTvosProxy;
            ouzelPoductRefGroup->children.push_back(libouzelTvosReferenceProxy);

            PBXObject ouzel(Id{0x30, 0x23, 0x20, 0x0C, 0x22, 0x22, 0x0B, 0xCF, 0x00, 0x7E, 0x0A, 0xAD});

            auto ouzelProxy = objects.alloc<PBXContainerItemProxy>();
            ouzelProxy->containerPortal = ouzelProjectFileRef;
            ouzelProxy->proxyType = PBXContainerItemProxy::Reference;
            ouzelProxy->remoteGlobal = &ouzel;
            ouzelProxy->remoteInfo = "ouzel";

            auto productFile = objects.alloc<PBXFileReference>();
            productFile->path = storage::Path{project.getName() + ".app"};
            productFile->fileType = PBXFileType::WrapperApplication;
            productFile->sourceTree = PBXSourceTree::BuildProductsDir;

            auto resourcesGroup = objects.alloc<PBXGroup>();
            resourcesGroup->path = storage::Path{"Resources"};
            resourcesGroup->sourceTree = PBXSourceTree::Group;
            mainGroup->children.push_back(resourcesGroup);

            auto productRefGroup = objects.alloc<PBXGroup>();
            productRefGroup->name = "Products";
            productRefGroup->children.push_back(productFile);
            productRefGroup->sourceTree = PBXSourceTree::Group;
            mainGroup->children.push_back(productRefGroup);
            pbxProject->productRefGroup = productRefGroup;

            std::vector<const PBXBuildFile*> buildFiles;
            std::vector<const PBXFileElement*> sourceFiles;

            for (auto sourceFile : project.getSourceFiles())
            {
                const auto extension = sourceFile.getExtension();

                auto fileReference = objects.alloc<PBXFileReference>();
                fileReference->path = sourceFile;
                // TODO: support more file formats
                fileReference->fileType = extension == "plist" ? PBXFileType::TextPlistXml :
                    extension == "c" ? PBXFileType::SourcecodeC :
                    extension == "h" ? PBXFileType::SourcecodeCH :
                    extension == "cpp" ? PBXFileType::SourcecodeCppCpp :
                    extension == "hpp" ? PBXFileType::SourcecodeCppH :
                    throw std::runtime_error("Unsupported file type");
                fileReference->sourceTree = PBXSourceTree::Group;
                sourceFiles.push_back(fileReference);

                auto buildFile = objects.alloc<PBXBuildFile>();
                buildFile->fileRef = fileReference;
                buildFiles.push_back(buildFile);
            }

            auto frameworksGroup = objects.alloc<PBXGroup>();
            frameworksGroup->name = "Frameworks";
            frameworksGroup->sourceTree = PBXSourceTree::Group;
            mainGroup->children.push_back(frameworksGroup);

            auto sourceGroup = objects.alloc<PBXGroup>();
            sourceGroup->name = "src";
            sourceGroup->path = storage::Path{"src"};
            sourceGroup->children = sourceFiles;
            sourceGroup->sourceTree = PBXSourceTree::Group;
            mainGroup->children.push_back(sourceGroup);

            auto projectConfigurationList = objects.alloc<XCConfigurationList>();
            pbxProject->buildConfigurationList = projectConfigurationList;

            const auto headerSearchPath = std::string(project.getOuzelPath() / "engine");
            auto debugConfiguration = objects.alloc<XCBuildConfiguration>();
            debugConfiguration->name = "Debug";
            debugConfiguration->buildSettings = {
                {"CLANG_CXX_LANGUAGE_STANDARD", "c++14"},
                {"CLANG_CXX_LIBRARY", "libc++"},
                {"ENABLE_TESTABILITY", "YES"},
                {"GCC_OPTIMIZATION_LEVEL", "0"},
                {"GCC_PREPROCESSOR_DEFINITIONS", "DEBUG"},
                {"HEADER_SEARCH_PATHS", headerSearchPath},
                {"ONLY_ACTIVE_ARCH", "YES"},
                {"CLANG_WARN_ASSIGN_ENUM", "YES"},
                {"CLANG_WARN_COMMA", "YES"},
                {"CLANG_WARN_EMPTY_BODY", "YES"},
                {"CLANG_WARN_IMPLICIT_SIGN_CONVERSION", "YES"},
                {"CLANG_WARN_INFINITE_RECURSION", "YES"},
                {"CLANG_WARN_QUOTED_INCLUDE_IN_FRAMEWORK_HEADER", "YES"},
                {"CLANG_WARN_SEMICOLON_BEFORE_METHOD_BODY", "YES"},
                {"CLANG_WARN_STRICT_PROTOTYPES", "YES"},
                {"CLANG_WARN_SUSPICIOUS_IMPLICIT_CONVERSION", "YES"},
                {"CLANG_WARN_SUSPICIOUS_MOVE", "YES"},
                {"CLANG_WARN_UNREACHABLE_CODE", "YES"},
                {"GCC_WARN_ABOUT_MISSING_FIELD_INITIALIZERS", "YES"},
                {"GCC_WARN_ABOUT_MISSING_NEWLINE", "YES"},
                {"GCC_WARN_ABOUT_MISSING_PROTOTYPES", "YES"},
                {"GCC_WARN_ABOUT_RETURN_TYPE", "YES"},
                {"GCC_WARN_FOUR_CHARACTER_CONSTANTS", "YES"},
                {"GCC_WARN_HIDDEN_VIRTUAL_FUNCTIONS", "YES"},
                {"GCC_WARN_INITIALIZER_NOT_FULLY_BRACKETED", "YES"},
                {"GCC_WARN_PEDANTIC", "YES"},
                {"GCC_WARN_SHADOW", "YES"},
                {"GCC_WARN_SIGN_COMPARE", "YES"},
                {"GCC_WARN_UNINITIALIZED_AUTOS", "YES_AGGRESSIVE"},
                {"GCC_WARN_UNKNOWN_PRAGMAS", "YES"},
                {"GCC_WARN_UNUSED_FUNCTION", "YES"},
                {"GCC_WARN_UNUSED_LABEL", "YES"},
                {"GCC_WARN_UNUSED_PARAMETER", "YES"},
                {"GCC_WARN_UNUSED_VARIABLE", "YES"}
            };

            projectConfigurationList->configurations.push_back(debugConfiguration);

            auto releaseConfiguration = objects.alloc<XCBuildConfiguration>();
            releaseConfiguration->name = "Release";
            releaseConfiguration->buildSettings = {
                {"CLANG_CXX_LANGUAGE_STANDARD", "c++14"},
                {"CLANG_CXX_LIBRARY", "libc++"},
                {"HEADER_SEARCH_PATHS", headerSearchPath},
                {"CLANG_WARN_ASSIGN_ENUM", "YES"},
                {"CLANG_WARN_COMMA", "YES"},
                {"CLANG_WARN_EMPTY_BODY", "YES"},
                {"CLANG_WARN_IMPLICIT_SIGN_CONVERSION", "YES"},
                {"CLANG_WARN_INFINITE_RECURSION", "YES"},
                {"CLANG_WARN_QUOTED_INCLUDE_IN_FRAMEWORK_HEADER", "YES"},
                {"CLANG_WARN_SEMICOLON_BEFORE_METHOD_BODY", "YES"},
                {"CLANG_WARN_STRICT_PROTOTYPES", "YES"},
                {"CLANG_WARN_SUSPICIOUS_IMPLICIT_CONVERSION", "YES"},
                {"CLANG_WARN_SUSPICIOUS_MOVE", "YES"},
                {"CLANG_WARN_UNREACHABLE_CODE", "YES"},
                {"GCC_WARN_ABOUT_MISSING_FIELD_INITIALIZERS", "YES"},
                {"GCC_WARN_ABOUT_MISSING_NEWLINE", "YES"},
                {"GCC_WARN_ABOUT_MISSING_PROTOTYPES", "YES"},
                {"GCC_WARN_ABOUT_RETURN_TYPE", "YES"},
                {"GCC_WARN_FOUR_CHARACTER_CONSTANTS", "YES"},
                {"GCC_WARN_HIDDEN_VIRTUAL_FUNCTIONS", "YES"},
                {"GCC_WARN_INITIALIZER_NOT_FULLY_BRACKETED", "YES"},
                {"GCC_WARN_PEDANTIC", "YES"},
                {"GCC_WARN_SHADOW", "YES"},
                {"GCC_WARN_SIGN_COMPARE", "YES"},
                {"GCC_WARN_UNINITIALIZED_AUTOS", "YES_AGGRESSIVE"},
                {"GCC_WARN_UNKNOWN_PRAGMAS", "YES"},
                {"GCC_WARN_UNUSED_FUNCTION", "YES"},
                {"GCC_WARN_UNUSED_LABEL", "YES"},
                {"GCC_WARN_UNUSED_PARAMETER", "YES"},
                {"GCC_WARN_UNUSED_VARIABLE", "YES"}
            };

            projectConfigurationList->configurations.push_back(releaseConfiguration);
            projectConfigurationList->defaultConfigurationName = releaseConfiguration->name;

            for (const auto& target : project.getTargets())
            {
                if (target.getPlatform() == Platform::MacOs ||
                    target.getPlatform() == Platform::Ios ||
                    target.getPlatform() == Platform::Tvos)
                {
                    auto nativeTarget = objects.alloc<PBXNativeTarget>();
                    nativeTarget->name = target.getName() + ' ' + toString(target.getPlatform());
                    nativeTarget->productReference = productFile;
                    pbxProject->targets.push_back(nativeTarget);

                    auto targetConfigurationList = objects.alloc<XCConfigurationList>();
                    nativeTarget->buildConfigurationList = targetConfigurationList;

                    auto targetDebugConfiguration = objects.alloc<XCBuildConfiguration>();
                    targetDebugConfiguration->name = "Debug";
                    targetConfigurationList->configurations.push_back(targetDebugConfiguration);

                    auto targetReleaseConfiguration = objects.alloc<XCBuildConfiguration>();
                    targetReleaseConfiguration->name = "Release";
                    targetConfigurationList->configurations.push_back(targetReleaseConfiguration);
                    targetConfigurationList->defaultConfigurationName = targetReleaseConfiguration->name;

                    auto sourcesBuildPhase = objects.alloc<PBXSourcesBuildPhase>();
                    sourcesBuildPhase->files = buildFiles;
                    nativeTarget->buildPhases.push_back(sourcesBuildPhase);

                    auto frameworksBuildPhase = objects.alloc<PBXFrameworksBuildPhase>();
                    nativeTarget->buildPhases.push_back(frameworksBuildPhase);

                    std::map<std::string, std::string> buildSettings = {
                        {"INFOPLIST_FILE", toString(target.getPlatform()) + "/Info.plist"},
                        {"PRODUCT_BUNDLE_IDENTIFIER", project.getIdentifier()},
                        {"PRODUCT_NAME", project.getName()}
                    };

                    auto infoPlistFileReference = objects.alloc<PBXFileReference>();
                    infoPlistFileReference->path = storage::Path{"Info.plist"};
                    infoPlistFileReference->fileType = PBXFileType::TextPlistXml;
                    infoPlistFileReference->sourceTree = PBXSourceTree::Group;

                    auto platformGroup = objects.alloc<PBXGroup>();
                    platformGroup->path = storage::Path{toString(target.getPlatform())};
                    platformGroup->children.push_back(infoPlistFileReference);
                    platformGroup->sourceTree = PBXSourceTree::Group;
                    mainGroup->children.push_back(platformGroup);

                    storage::Path sdkPath;
                    PBXSourceTree frameworkSourceTree = PBXSourceTree::SdkRoot;
                    std::set<const char*> frameworks;

                    const auto platformDirectory = projectDirectory / toString(target.getPlatform());
                    const auto plistPath = platformDirectory / "Info.plist";
                    const auto platformDirectoryType = platformDirectory.getType();
                    if (platformDirectoryType == storage::Path::Type::NotFound)
                        storage::FileSystem::createDirectory(platformDirectory);
                    else if (platformDirectoryType != storage::Path::Type::Directory)
                    {
                        storage::FileSystem::deleteFile(platformDirectory);
                        storage::FileSystem::createDirectory(platformDirectory);
                    }

                    switch (target.getPlatform())
                    {
                        case Platform::MacOs:
                        {
                            buildSettings["SDKROOT"] = "macosx";
                            buildSettings["MACOSX_DEPLOYMENT_TARGET"] = "10.8";
                            frameworks = {"AudioToolbox.framework",
                                "AudioUnit.framework",
                                "Cocoa.framework",
                                "CoreAudio.framework",
                                "CoreVideo.framework",
                                "GameController.framework",
                                "IOKit.framework",
                                "Metal.framework",
                                "OpenAL.framework",
                                "OpenGL.framework",
                                "QuartzCore.framework"};
                            auto libouzelMacOsBuildFile = objects.alloc<PBXBuildFile>();
                            libouzelMacOsBuildFile->fileRef = libouzelMacOsReferenceProxy;
                            frameworksBuildPhase->files.push_back(libouzelMacOsBuildFile);

                            plist::Value infoPlist = plist::Value::Dictionary{
                                {"CFBundleDevelopmentRegion", "en"},
                                {"CFBundleExecutable", "$(EXECUTABLE_NAME)"},
                                {"CFBundleIconFile", ""},
                                {"CFBundleIdentifier", "$(PRODUCT_BUNDLE_IDENTIFIER)"},
                                {"CFBundleInfoDictionaryVersion", "6.0"},
                                {"CFBundleName", "$(PRODUCT_NAME)"},
                                {"CFBundlePackageType", "APPL"},
                                {"CFBundleShortVersionString", "1.0"},
                                {"CFBundleSignature", "????"},
                                {"CFBundleVersion", "1"},
                                {"LSMinimumSystemVersion", "$(MACOSX_DEPLOYMENT_TARGET)"},
                                {"NSHumanReadableCopyright", "Copyright " + project.getOrganization() + ". All rights reserved."},
                                {"NSPrincipalClass", "NSApplication"}
                            };

                            std::ofstream infoPlistFile(plistPath);
                            infoPlistFile << plist::encode(infoPlist, plist::Format::Xml);
                            break;
                        }

                        case Platform::Ios:
                        {
                            buildSettings["SDKROOT"] = "iphoneos";
                            buildSettings["IPHONEOS_DEPLOYMENT_TARGET"] = "8.0";
                            buildSettings["TARGETED_DEVICE_FAMILY"] = "1,2";
                            frameworks = {"AudioToolbox.framework",
                                "AVFoundation.framework",
                                "Foundation.framework",
                                "GameController.framework",
                                "Metal.framework",
                                "OpenAL.framework",
                                "OpenGLES.framework",
                                "QuartzCore.framework",
                                "UIKit.framework"
                            };
                            sdkPath = "Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk";
                            frameworkSourceTree = PBXSourceTree::DeveloperDir;
                            auto libouzelIosBuildFile = objects.alloc<PBXBuildFile>();
                            libouzelIosBuildFile->fileRef = libouzelIosReferenceProxy;
                            frameworksBuildPhase->files.push_back(libouzelIosBuildFile);

                            plist::Value infoPlist = plist::Value::Dictionary{
                                {"CFBundleDevelopmentRegion", "en"},
                                {"CFBundleExecutable", "$(EXECUTABLE_NAME)"},
                                {"CFBundleIdentifier", "$(PRODUCT_BUNDLE_IDENTIFIER)"},
                                {"CFBundleInfoDictionaryVersion", "6.0"},
                                {"CFBundleName", "$(PRODUCT_NAME)"},
                                {"CFBundlePackageType", "APPL"},
                                {"CFBundleShortVersionString", "1.0"},
                                {"CFBundleSignature", "????"},
                                {"CFBundleVersion", "1"},
                                {"LSRequiresIPhoneOS", true},
                                {"UILaunchStoryboardName", "LaunchScreen"},
                                {"UIRequiredDeviceCapabilities", plist::Value::Array{"armv7"}},
                                {"UIRequiresFullScreen", true},
                                {"UIStatusBarHidden", true},
                                {"UISupportedInterfaceOrientations", plist::Value::Array{
                                    "UIInterfaceOrientationPortrait",
                                    "UIInterfaceOrientationPortraitUpsideDown",
                                    "UIInterfaceOrientationLandscapeLeft",
                                    "UIInterfaceOrientationLandscapeRight"
                                }},
                                {"UISupportedInterfaceOrientations~ipad", plist::Value::Array{
                                    "UIInterfaceOrientationPortrait",
                                    "UIInterfaceOrientationPortraitUpsideDown",
                                    "UIInterfaceOrientationLandscapeLeft",
                                    "UIInterfaceOrientationLandscapeRight"
                                }}
                            };

                            std::ofstream infoPlistFile(plistPath);
                            infoPlistFile << plist::encode(infoPlist, plist::Format::Xml);
                            break;
                        }

                        case Platform::Tvos:
                        {
                            buildSettings["SDKROOT"] = "appletvos";
                            buildSettings["TVOS_DEPLOYMENT_TARGET"] = "9.0";
                            frameworks = {"AudioToolbox.framework",
                                "AVFoundation.framework",
                                "Foundation.framework",
                                "GameController.framework",
                                "Metal.framework",
                                "OpenAL.framework",
                                "OpenGLES.framework",
                                "QuartzCore.framework",
                                "UIKit.framework"
                            };
                            sdkPath = "Platforms/AppleTVOS.platform/Developer/SDKs/AppleTVOS.sdk";
                            frameworkSourceTree = PBXSourceTree::DeveloperDir;
                            auto libouzelTvosBuildFile = objects.alloc<PBXBuildFile>();
                            libouzelTvosBuildFile->fileRef = libouzelTvosReferenceProxy;
                            frameworksBuildPhase->files.push_back(libouzelTvosBuildFile);

                            plist::Value infoPlist = plist::Value::Dictionary{
                                {"CFBundleDevelopmentRegion", "en"},
                                {"CFBundleExecutable", "$(EXECUTABLE_NAME)"},
                                {"CFBundleIdentifier", "$(PRODUCT_BUNDLE_IDENTIFIER)"},
                                {"CFBundleInfoDictionaryVersion", "6.0"},
                                {"CFBundleName", "$(PRODUCT_NAME)"},
                                {"CFBundlePackageType", "APPL"},
                                {"CFBundleShortVersionString", "1.0"},
                                {"CFBundleSignature", "????"},
                                {"CFBundleVersion", "1"},
                                {"LSRequiresIPhoneOS", true},
                                {"UILaunchStoryboardName", "LaunchScreen"},
                                {"UIRequiredDeviceCapabilities", plist::Value::Array{"armv7"}}
                            };

                            std::ofstream infoPlistFile(plistPath);
                            infoPlistFile << plist::encode(infoPlist, plist::Format::Xml);
                            break;
                        }

                        default:
                            throw std::runtime_error("Unsupported platform");
                    }

                    targetDebugConfiguration->buildSettings = buildSettings;
                    targetReleaseConfiguration->buildSettings = buildSettings;

                    const auto frameworksPath = sdkPath / storage::Path{"System/Library/Frameworks"};
                    for (auto framework : frameworks)
                    {
                        auto frameworkFileReference = objects.alloc<PBXFileReference>();
                        frameworkFileReference->name = framework;
                        frameworkFileReference->path = frameworksPath / framework;
                        frameworkFileReference->fileType = PBXFileType::WrapperFramework;
                        frameworkFileReference->sourceTree = frameworkSourceTree;
                        frameworksGroup->children.push_back(frameworkFileReference);

                        auto frameworkBuildFile = objects.alloc<PBXBuildFile>();
                        frameworkBuildFile->fileRef = frameworkFileReference;
                        frameworksBuildPhase->files.push_back(frameworkBuildFile);
                    }

                    // TODO: implement resource copy
                    auto resourcesBuildPhase = objects.alloc<PBXResourcesBuildPhase>();
                    nativeTarget->buildPhases.push_back(resourcesBuildPhase);

                    auto ouzelNativeTargetProxy = objects.alloc<PBXContainerItemProxy>();
                    ouzelNativeTargetProxy->containerPortal = ouzelProjectFileRef;
                    ouzelNativeTargetProxy->proxyType = PBXContainerItemProxy::NativeTarget;
                    ouzelNativeTargetProxy->remoteGlobal = &ouzel;
                    ouzelNativeTargetProxy->remoteInfo = "ouzel";

                    auto ouzelReferenceProxy = objects.alloc<PBXReferenceProxy>();
                    ouzelReferenceProxy->path = "ouzel";
                    ouzelReferenceProxy->fileType = PBXFileType::CompiledMachOExecutable;
                    ouzelReferenceProxy->sourceTree = PBXSourceTree::BuildProductsDir;
                    ouzelReferenceProxy->remoteRef = ouzelProxy;

                    auto ouzelDependency = objects.alloc<PBXTargetDependency>();
                    ouzelDependency->name = "ouzel";
                    ouzelDependency->targetProxy = ouzelNativeTargetProxy;

                    auto assetGenerateTarget = objects.alloc<PBXLegacyTarget>();
                    assetGenerateTarget->name = "Generate Assets " + toString(target.getPlatform());
                    assetGenerateTarget->buildToolPath = "$BUILT_PRODUCTS_DIR/ouzel";
                    assetGenerateTarget->buildArgumentsString = "--export-assets $PROJECT_DIR/" + std::string(projectFilename); // TODO: add target
                    assetGenerateTarget->dependencies.push_back(ouzelDependency);
                    pbxProject->targets.push_back(assetGenerateTarget);

                    auto assetGenerateTargetConfigurationList = objects.alloc<XCConfigurationList>();
                    assetGenerateTargetConfigurationList->configurations.push_back(targetDebugConfiguration);
                    assetGenerateTargetConfigurationList->configurations.push_back(targetReleaseConfiguration);
                    assetGenerateTargetConfigurationList->defaultConfigurationName = targetReleaseConfiguration->name;
                    assetGenerateTarget->buildConfigurationList = assetGenerateTargetConfigurationList;

                    auto assetGenerateTargetProxy = objects.alloc<PBXContainerItemProxy>();
                    assetGenerateTargetProxy->proxyType = PBXContainerItemProxy::NativeTarget;
                    assetGenerateTargetProxy->containerPortal = pbxProject;
                    assetGenerateTargetProxy->remoteGlobal = assetGenerateTarget;
                    assetGenerateTargetProxy->remoteInfo = assetGenerateTarget->name;

                    auto assetGenerateTargetDependency = objects.alloc<PBXTargetDependency>();
                    assetGenerateTargetDependency->targetProxy = assetGenerateTargetProxy;
                    assetGenerateTargetDependency->target = assetGenerateTarget;
                    nativeTarget->dependencies.push_back(assetGenerateTargetDependency);
                }
            }

            const auto pbxProjectFile = xcodeProjectDirectory / storage::Path{"project.pbxproj"};
            std::ofstream file(pbxProjectFile, std::ios::trunc);

            plist::Value result = plist::Value::Dictionary{
                {"archiveVersion", 1},
                {"classes", plist::Value::Dictionary{}},
                {"objectVersion", 50},
                {"objects", plist::Value::Dictionary{}},
                {"rootObject", toString(pbxProject->getId())}
            };

            for (const auto& object : objects.getObjects())
                result["objects"][toString(object->getId())] = object->encode();

            file << plist::encode(result, plist::Format::Ascii);
        }
    }
}

#endif // OUZEL_XCODE_PROJECT_HPP
