#
# Build configuration for the ZAnalysis package
#

# Set the name of the package:
atlas_subdir( ZAnalysis )

# Set up which packages this package depends on:
atlas_depends_on_subdirs(
   PUBLIC
   Control/AthToolSupport/AsgTools
   PhysicsAnalysis/AnalysisCommon/PATCore
   PhysicsAnalysis/D3PDTools/EventLoop
   PhysicsAnalysis/ElectronPhotonID/ElectronPhotonFourMomentumCorrection
   PhysicsAnalysis/Interfaces/AsgAnalysisInterfaces
   PRIVATE
   Control/AthContainers
   Control/CxxUtils
   Control/xAODRootAccess
   Event/xAOD/xAODBase
   Event/xAOD/xAODCore
   Event/xAOD/xAODEgamma
   Event/xAOD/xAODEventInfo
   Event/xAOD/xAODTracking )

# External(s) used by the package:
find_package( ROOT COMPONENTS Core Hist Physics REQUIRED )

# Generate a dictionary for the library:
atlas_add_root_dictionary( ZAnalysisLib ZAnalysisLibDictSrc
   ROOT_HEADERS ZAnalysis/ElectronCalibAlg.h ZAnalysis/ElectronSelectionAlg.h
   ZAnalysis/ZAnalysisAlg.h Root/LinkDef.h
   EXTERNAL_PACKAGES ROOT )

# Build the shared library of the package:
atlas_add_library( ZAnalysisLib
   ZAnalysis/*.h Root/*.h Root/*.cxx ${ZAnalysisLibDictSrc}
   PUBLIC_HEADERS ZAnalysis
   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
   LINK_LIBRARIES AsgTools PATCoreLib EventLoop
   ElectronPhotonFourMomentumCorrectionLib AsgAnalysisInterfaces
   PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES} AthContainers CxxUtils
   xAODRootAccess xAODBase xAODCore xAODEgamma xAODEventInfo xAODTracking )

# Install the python script(s) of the package:
atlas_install_scripts( macros/LocalJob.py )
