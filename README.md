# Unreal Binaries Rebuild Helper Tool
## Description
  This tool can be used for Unreal Engine Projects version 4.27. We have one issue with Binaries rebuild for version 4.27. 
When I was trying to build it using original way via context menu, I recieved error. In 4.27 build something wrong happened
with pathes to unreal build utilities. Despite of it we need to clear some bin data. For example Binaries, Intermediate and Saved 
folders. And it can be the silver bulet to solve this issues. I decided to create the automation tool that will clean old bin data 
and rebuild all binaries using UnrealBuildTool.exe that is delivered with the Unreal Engine 4.
## How to use it
  - We need to locate the absolute path to the UnrealBuildTool.exe file. It is located here "..\UE_4.27\Engine\Binaries\DotNET".
    Be careful your tool can be placed in another location it depends on where the UE 4 installed.
  - Then you need to create User scoped environment variable with name: UT_PATH, something like UnrealTools path.
  - Clone this repo.
  - Build it using Visual Studio 2022. Better to use Release option for build.
  - Then go to x64 folder that will be created after build. Navigate to the rlease or debug,
    that depends on build configuration choosen in the previous step. Here you will see UnrealBinariesRebuildHelperTool.exe
    Copy it and paste to some Unreal Progect that must be rebuild.
    It must be located in the same folder as your *.uproject file.
    <img width="762" height="362" alt="image" src="https://github.com/user-attachments/assets/83459b02-b686-4de7-afcb-d4f6e187ff34" />
  - Launch it. If you see this output that is ok. That means that we get the path to Unreal Utilities.
    <img width="741" height="253" alt="image" src="https://github.com/user-attachments/assets/f44e08e3-653b-49f1-a1e6-5bc8ca607482" />
  - Provide the name of the *.uproject for rebuild and hit Enter. In my case I'd like to rebuild Test project.
  - And we get the output:
    <img width="1142" height="621" alt="image" src="https://github.com/user-attachments/assets/e1dd9c61-04f1-4a6b-8cd3-68f726586498" />
    That means that all is good.
  - Sometimes you can see:
    <img width="542" height="33" alt="image" src="https://github.com/user-attachments/assets/53c16127-78fc-436d-8d38-e09b3dd20071" />
    But it's ok. That means that there was no folder for removal.

    



