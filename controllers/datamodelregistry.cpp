#include "DataModelRegistry.h"

DataModelRegistry::DataModelRegistry(QObject *parent) : QObject(parent) {

}

void DataModelRegistry::refreshAllModels() {
    //Operation Stats
    modelRecentActivity->refreshQuery("SELECT * FROM View_RecentActivity");
    modelTotalOps->refreshQuery("SELECT * FROM View_TotalOperations");
    modelDuration->refreshQuery("SELECT * FROM View_OperationDuration");
    modelDurationSummary->refreshQuery("SELECT * FROM View_OpDurationSummary");
    modelBreakdown->refreshQuery("SELECT * FROM View_OperationBreakdown");
    modelRequestorMode->refreshQuery("SELECT * FROM View_RequestorModeCount");
    modelOpStatus->refreshQuery("SELECT * FROM View_OpStatusCount");
    modelOprTypeByMode->refreshQuery("SELECT * FROM View_OpTypeByRequestor");
    modelMajorOpByMode->refreshQuery("SELECT * FROM View_MajorOpByRequestor");
    //File Stats
    modelFileCounts->refreshQuery("SELECT * FROM View_FileOpCounts");
    modelFileWriteTiming->refreshQuery("SELECT * FROM View_FileWriteTiming");
    //Process Stats
    modelProcessCounts->refreshQuery("SELECT * FROM View_ProcessOpCounts");
    modelThreadBreakdown->refreshQuery("SELECT * FROM View_ThreadBreakdown");
}
