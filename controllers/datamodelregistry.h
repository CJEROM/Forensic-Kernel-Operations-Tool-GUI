#ifndef DATAMODELREGISTRY_H
#define DATAMODELREGISTRY_H

#pragma once

#include <QObject>
#include "MinifilterLogModel.h"

class DataModelRegistry : public QObject {
    Q_OBJECT

    // ✅ Expose as properties
    Q_PROPERTY(MinifilterLogModel* modelRecentActivity READ modelRecentActivity CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelTotalOps READ modelTotalOps CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelTotalAlerts READ modelTotalAlerts CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelDuration READ modelDuration CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelDurationSummary READ modelDurationSummary CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelBreakdown READ modelBreakdown CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelRequestorMode READ modelRequestorMode CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelOpStatus READ modelOpStatus CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelOprTypeByMode READ modelOprTypeByMode CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelMajorOpByMode READ modelMajorOpByMode CONSTANT)

    Q_PROPERTY(MinifilterLogModel* modelFileCounts READ modelFileCounts CONSTANT)

    Q_PROPERTY(MinifilterLogModel* modelProcessCounts READ modelProcessCounts CONSTANT)
    Q_PROPERTY(MinifilterLogModel* modelThreadBreakdown READ modelThreadBreakdown CONSTANT)
public:
    explicit DataModelRegistry(QObject *parent = nullptr);

    Q_INVOKABLE void refreshAllModels();

    // Getters
    Q_INVOKABLE MinifilterLogModel *modelRecentActivity() const { return m_modelRecentActivity; }
    Q_INVOKABLE MinifilterLogModel *modelTotalOps() const { return m_modelTotalOps; }
    Q_INVOKABLE MinifilterLogModel *modelTotalAlerts() const { return m_modelTotalAlerts; }
    Q_INVOKABLE MinifilterLogModel *modelDuration() const { return m_modelDuration; }
    Q_INVOKABLE MinifilterLogModel *modelDurationSummary() const { return m_modelDurationSummary; }
    Q_INVOKABLE MinifilterLogModel *modelBreakdown() const { return m_modelBreakdown; }
    Q_INVOKABLE MinifilterLogModel *modelRequestorMode() const { return m_modelRequestorMode; }
    Q_INVOKABLE MinifilterLogModel *modelOpStatus() const { return m_modelOpStatus; }
    Q_INVOKABLE MinifilterLogModel *modelOprTypeByMode() const { return m_modelOprTypeByMode; }
    Q_INVOKABLE MinifilterLogModel *modelMajorOpByMode() const { return m_modelMajorOpByMode; }

    Q_INVOKABLE MinifilterLogModel *modelFileCounts() const { return m_modelFileCounts; }

    Q_INVOKABLE MinifilterLogModel *modelProcessCounts() const { return m_modelProcessCounts; }
    Q_INVOKABLE MinifilterLogModel *modelThreadBreakdown() const { return m_modelThreadBreakdown; }

private:
    // Declare internal pointers
    MinifilterLogModel *m_modelRecentActivity;
    MinifilterLogModel *m_modelTotalOps;
    MinifilterLogModel *m_modelTotalAlerts;
    MinifilterLogModel *m_modelDuration;
    MinifilterLogModel *m_modelDurationSummary;
    MinifilterLogModel *m_modelBreakdown;
    MinifilterLogModel *m_modelRequestorMode;
    MinifilterLogModel *m_modelOpStatus;
    MinifilterLogModel *m_modelOprTypeByMode;
    MinifilterLogModel *m_modelMajorOpByMode;

    MinifilterLogModel *m_modelFileCounts;

    MinifilterLogModel *m_modelProcessCounts;
    MinifilterLogModel *m_modelThreadBreakdown;
};

#endif // DATAMODELREGISTRY_H
