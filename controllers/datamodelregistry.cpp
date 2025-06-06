#include "DataModelRegistry.h"

DataModelRegistry::DataModelRegistry(QObject *parent) : QObject(parent) {
    //Operation Stats
    m_modelRecentActivity = new MinifilterLogModel(this);
    m_modelTotalOps = new MinifilterLogModel(this);
    m_modelTotalAlerts = new MinifilterLogModel(this);
    m_modelDuration = new MinifilterLogModel(this);
    m_modelDurationSummary = new MinifilterLogModel(this);
    m_modelBreakdown = new MinifilterLogModel(this);
    m_modelRequestorMode = new MinifilterLogModel(this);
    m_modelOpStatus = new MinifilterLogModel(this);
    m_modelOprTypeByMode = new MinifilterLogModel(this);
    m_modelMajorOpByMode = new MinifilterLogModel(this);
    //File Stats
    m_modelFileCounts = new MinifilterLogModel(this);
    //Process Stats
    m_modelProcessCounts = new MinifilterLogModel(this);
    m_modelThreadBreakdown = new MinifilterLogModel(this);

    // refreshAllModels(); // This needs to be called later as well
}

void DataModelRegistry::refreshAllModels() {
    //Operation Stats
    m_modelRecentActivity->refreshQuery("SELECT TimeBucket * 5 AS TimeInSeconds, MajorOp, Count FROM View_RecentOpsGrouped ORDER BY TimeInSeconds ASC");
    m_modelTotalOps->refreshQuery("SELECT * FROM View_TotalOperations");
    m_modelTotalAlerts->refreshQuery("SELECT * FROM View_TotalAlerts");
    m_modelDuration->refreshQuery("SELECT * FROM View_OperationDuration");
    m_modelDurationSummary->refreshQuery("SELECT * FROM View_OpDurationSummary");
    m_modelBreakdown->refreshQuery("SELECT * FROM View_OperationBreakdown");
    m_modelRequestorMode->refreshQuery("SELECT * FROM View_RequestorModeCount");
    m_modelOpStatus->refreshQuery("SELECT * FROM View_OpStatusCount");
    m_modelOprTypeByMode->refreshQuery("SELECT * FROM View_OpTypeByRequestor");
    m_modelMajorOpByMode->refreshQuery("SELECT * FROM View_MajorOpByRequestor");
    //File Stats
    m_modelFileCounts->refreshQuery("SELECT * FROM View_FileOpCounts");
    //Process Stats
    m_modelProcessCounts->refreshQuery("SELECT * FROM View_ProcessOpCounts");
    m_modelThreadBreakdown->refreshQuery("SELECT * FROM View_ThreadBreakdown");
}
