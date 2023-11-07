@RD /S /Q .vs
@RD /S /Q Binaries
@RD /S /Q DerivedDataCache
@RD /S /Q Intermediate
@RD /S /Q Saved
@RD /S /Q Script
@DEL /S /Q Aura.sln

"D:\UnrealEngine\UE_5.2\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="D:\UnrealEngine\Projects\Aura" -game -rocket -progress

REM .git, config, content, source, i uproject stay put