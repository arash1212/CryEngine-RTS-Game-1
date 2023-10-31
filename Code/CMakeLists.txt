cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "D:/CRYENGINE Launcher/Engines/crytek/cryengine-57-lts/5.7.1" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"GamePlugin.h"
		"StdAfx.h"
)
add_sources("Actions_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Actions"
		"Actions/IBaseAction.h"
)
add_sources("Train_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Actions\\\\Train"
		"Actions/Train/TrainEngineer1Action.cpp"
		"Actions/Train/TrainSoldier1Action.cpp"
		"Actions/Train/TrainEngineer1Action.h"
		"Actions/Train/TrainSoldier1Action.h"
)
add_sources("Units_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Actions\\\\Units"
		"Actions/Units/UnitAttackAction.cpp"
		"Actions/Units/UnitBuildAction.cpp"
		"Actions/Units/UnitCollectResourceAction.cpp"
		"Actions/Units/UnitMoveAction.cpp"
		"Actions/Units/UnitSendResourceToWarehouseAction.cpp"
		"Actions/Units/UnitWanderingRandomlyAction.cpp"
		"Actions/Units/UnitAttackAction.h"
		"Actions/Units/UnitBuildAction.h"
		"Actions/Units/UnitCollectResourceAction.h"
		"Actions/Units/UnitMoveAction.h"
		"Actions/Units/UnitSendResourceToWarehouseAction.h"
		"Actions/Units/UnitWanderingRandomlyAction.h"
)
add_sources("Components_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components"
)
add_sources("BaseBuilding_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\BaseBuilding"
		"Components/BaseBuilding/BaseBuilding.cpp"
		"Components/BaseBuilding/Building.cpp"
		"Components/BaseBuilding/BaseBuilding.h"
		"Components/BaseBuilding/Building.h"
)
add_sources("Buildings_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\BaseBuilding\\\\Buildings"
		"Components/BaseBuilding/Buildings/Alchemy1Building.cpp"
		"Components/BaseBuilding/Buildings/Bakery1Building.cpp"
		"Components/BaseBuilding/Buildings/Barracks1Building.cpp"
		"Components/BaseBuilding/Buildings/BulletFactory1Building.cpp"
		"Components/BaseBuilding/Buildings/Cave1Building.cpp"
		"Components/BaseBuilding/Buildings/Farm1Building.cpp"
		"Components/BaseBuilding/Buildings/GunPowderFactory1Building.cpp"
		"Components/BaseBuilding/Buildings/House1Building.cpp"
		"Components/BaseBuilding/Buildings/HQ1Building.cpp"
		"Components/BaseBuilding/Buildings/Light1Building.cpp"
		"Components/BaseBuilding/Buildings/Warehouse1Building.cpp"
		"Components/BaseBuilding/Buildings/Windmill1Building.cpp"
		"Components/BaseBuilding/Buildings/Alchemy1Building.h"
		"Components/BaseBuilding/Buildings/Bakery1Building.h"
		"Components/BaseBuilding/Buildings/Barracks1Building.h"
		"Components/BaseBuilding/Buildings/BulletFactory1Building.h"
		"Components/BaseBuilding/Buildings/Cave1Building.h"
		"Components/BaseBuilding/Buildings/Farm1Building.h"
		"Components/BaseBuilding/Buildings/GunPowderFactory1Building.h"
		"Components/BaseBuilding/Buildings/House1Building.h"
		"Components/BaseBuilding/Buildings/HQ1Building.h"
		"Components/BaseBuilding/Buildings/Light1Building.h"
		"Components/BaseBuilding/Buildings/Warehouse1Building.h"
		"Components/BaseBuilding/Buildings/Windmill1Building.h"
)
add_sources("Controller_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Controller"
		"Components/Controller/AIController.cpp"
		"Components/Controller/AIController.h"
)
add_sources("Effects_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Effects"
		"Components/Effects/BulletTracer.cpp"
		"Components/Effects/BulletTracer.h"
)
add_sources("Info_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Info"
		"Components/Info/OwnerInfo.cpp"
		"Components/Info/OwnerInfo.h"
)
add_sources("Managers_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Managers"
		"Components/Managers/ActionManager.cpp"
		"Components/Managers/ResourceManager.cpp"
		"Components/Managers/UnitStateManager.cpp"
		"Components/Managers/UnitTypeManager.cpp"
		"Components/Managers/ActionManager.h"
		"Components/Managers/ResourceManager.h"
		"Components/Managers/UnitStateManager.h"
		"Components/Managers/UnitTypeManager.h"
)
add_sources("Player_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Player"
		"Components/Player/Player.cpp"
		"Components/Player/Player.h"
)
add_sources("Resources_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Resources"
		"Components/Resources/Resource.cpp"
		"Components/Resources/Resource.h"
)
add_sources("Resource_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Resources\\\\Resource"
		"Components/Resources/Resource/ResourceIron.cpp"
		"Components/Resources/Resource/ResourceOil.cpp"
		"Components/Resources/Resource/ResourceWood.cpp"
		"Components/Resources/Resource/ResourceIron.h"
		"Components/Resources/Resource/ResourceOil.h"
		"Components/Resources/Resource/ResourceWood.h"
)
add_sources("Selectables_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Selectables"
		"Components/Selectables/Attacker.cpp"
		"Components/Selectables/Cost.cpp"
		"Components/Selectables/Engineer.cpp"
		"Components/Selectables/Health.cpp"
		"Components/Selectables/ResourceCollector.cpp"
		"Components/Selectables/ResourceStorage.cpp"
		"Components/Selectables/Selectable.cpp"
		"Components/Selectables/UnitAnimation.cpp"
		"Components/Selectables/Worker.cpp"
		"Components/Selectables/Workplace.cpp"
		"Components/Selectables/Attacker.h"
		"Components/Selectables/Cost.h"
		"Components/Selectables/Engineer.h"
		"Components/Selectables/Health.h"
		"Components/Selectables/ResourceCollector.h"
		"Components/Selectables/ResourceStorage.h"
		"Components/Selectables/Selectable.h"
		"Components/Selectables/UnitAnimation.h"
		"Components/Selectables/Worker.h"
		"Components/Selectables/Workplace.h"
)
add_sources("Units_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Selectables\\\\Units"
		"Components/Selectables/Units/Engineer1Unit.cpp"
		"Components/Selectables/Units/Soldier1Unit.cpp"
		"Components/Selectables/Units/Zombie1Unit.cpp"
		"Components/Selectables/Units/Engineer1Unit.h"
		"Components/Selectables/Units/Soldier1Unit.h"
		"Components/Selectables/Units/Zombie1Unit.h"
)
add_sources("Spawners_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Spawners"
		"Components/Spawners/SpawnPoint.cpp"
		"Components/Spawners/ZombieSpawner.cpp"
		"Components/Spawners/SpawnPoint.h"
		"Components/Spawners/ZombieSpawner.h"
)
add_sources("UI_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\UI"
		"Components/UI/UIActionbar.cpp"
		"Components/UI/UIHealthbars.cpp"
		"Components/UI/UIResourcesPanel.cpp"
		"Components/UI/UISelectionBox.cpp"
		"Components/UI/UIActionbar.h"
		"Components/UI/UIHealthbars.h"
		"Components/UI/UIResourcesPanel.h"
		"Components/UI/UISelectionBox.h"
)
add_sources("Listener_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\UI\\\\Listener"
		"Components/UI/Listener/UIElementEventListener.cpp"
		"Components/UI/Listener/UIElementEventListener.h"
)
add_sources("Weapons_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Weapons"
		"Components/Weapons/BaseWeapon.cpp"
		"Components/Weapons/BaseWeapon.h"
)
add_sources("UIItems_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems"
		"UIItems/IBaseUIItem.h"
)
add_sources("Items_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems\\\\Items"
		"UIItems/Items/UICancelItem.cpp"
		"UIItems/Items/UIChangeStanceItem.cpp"
		"UIItems/Items/UICancelItem.h"
		"UIItems/Items/UIChangeStanceItem.h"
)
add_sources("Buildings_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems\\\\Items\\\\Buildings"
		"UIItems/Items/Buildings/UIAK47Factory1BuildItem.cpp"
		"UIItems/Items/Buildings/UIAlchemy1BuildItem.cpp"
		"UIItems/Items/Buildings/UIBakery1BuildItem.cpp"
		"UIItems/Items/Buildings/UIBarracks1BuildItem.cpp"
		"UIItems/Items/Buildings/UIBulletFactory1BuildItem.cpp"
		"UIItems/Items/Buildings/UICave1BuildItem.cpp"
		"UIItems/Items/Buildings/UIFarm1BuildItem.cpp"
		"UIItems/Items/Buildings/UIGunPowderFactory1BuildItem.cpp"
		"UIItems/Items/Buildings/UIHouse1BuildItem.cpp"
		"UIItems/Items/Buildings/UIHQ1BuildItem.cpp"
		"UIItems/Items/Buildings/UILight1BuildItem.cpp"
		"UIItems/Items/Buildings/UIWarehouse1BuildItem.cpp"
		"UIItems/Items/Buildings/UIWindmillBuildItem.cpp"
		"UIItems/Items/Buildings/UIAK47Factory1BuildItem.h"
		"UIItems/Items/Buildings/UIAlchemy1BuildItem.h"
		"UIItems/Items/Buildings/UIBakery1BuildItem.h"
		"UIItems/Items/Buildings/UIBarracks1BuildItem.h"
		"UIItems/Items/Buildings/UIBulletFactory1BuildItem.h"
		"UIItems/Items/Buildings/UICave1BuildItem.h"
		"UIItems/Items/Buildings/UIFarm1BuildItem.h"
		"UIItems/Items/Buildings/UIGunPowderFactory1BuildItem.h"
		"UIItems/Items/Buildings/UIHouse1BuildItem.h"
		"UIItems/Items/Buildings/UIHQ1BuildItem.h"
		"UIItems/Items/Buildings/UILight1BuildItem.h"
		"UIItems/Items/Buildings/UIWarehouse1BuildItem.h"
		"UIItems/Items/Buildings/UIWindmillBuildItem.h"
)
add_sources("TrainUnits_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems\\\\Items\\\\Buildings\\\\TrainUnits"
		"UIItems/Items/Buildings/TrainUnits/UITrainEngineer1Item.cpp"
		"UIItems/Items/Buildings/TrainUnits/UITrainSoldier1Item.cpp"
		"UIItems/Items/Buildings/TrainUnits/UITrainEngineer1Item.h"
		"UIItems/Items/Buildings/TrainUnits/UITrainSoldier1Item.h"
)
add_sources("Resources_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIItems\\\\Items\\\\Resources"
		"UIItems/Items/Resources/UIBuyBreadItem.cpp"
		"UIItems/Items/Resources/UIBuyFlourItem.cpp"
		"UIItems/Items/Resources/UIBuyOilItem.cpp"
		"UIItems/Items/Resources/UIBuyWheatItem.cpp"
		"UIItems/Items/Resources/UIBuyWoodItem.cpp"
		"UIItems/Items/Resources/UISellBreadItem.cpp"
		"UIItems/Items/Resources/UISellFlourItem.cpp"
		"UIItems/Items/Resources/UISellOilItem.cpp"
		"UIItems/Items/Resources/UISellWheatItem.cpp"
		"UIItems/Items/Resources/UISellWoodItem.cpp"
		"UIItems/Items/Resources/UIBuyBreadItem.h"
		"UIItems/Items/Resources/UIBuyFlourItem.h"
		"UIItems/Items/Resources/UIBuyOilItem.h"
		"UIItems/Items/Resources/UIBuyWheatItem.h"
		"UIItems/Items/Resources/UIBuyWoodItem.h"
		"UIItems/Items/Resources/UISellBreadItem.h"
		"UIItems/Items/Resources/UISellFlourItem.h"
		"UIItems/Items/Resources/UISellOilItem.h"
		"UIItems/Items/Resources/UISellWheatItem.h"
		"UIItems/Items/Resources/UISellWoodItem.h"
)
add_sources("Utils_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Utils"
		"Utils/EntityUtils.cpp"
		"Utils/MathUtils.cpp"
		"Utils/MouseUtils.cpp"
		"Utils/EntityUtils.h"
		"Utils/MathUtils.h"
		"Utils/MouseUtils.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(Game FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM