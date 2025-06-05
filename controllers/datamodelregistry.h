#ifndef DATAMODELREGISTRY_H
#define DATAMODELREGISTRY_H

#pragma once

#include <QObject>
#include "MinifilterLogModel.h"

class DataModelRegistry : public QObject {
    Q_OBJECT
public:
    explicit DataModelRegistry(QObject *parent = nullptr);

    Q_INVOKABLE void refreshAllModels();

    // Pointers to global models
    //Operation Stats
    Q_INVOKABLE MinifilterLogModel *modelRecentActivity = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelTotalOps = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelDuration = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelDurationSummary = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelBreakdown = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelRequestorMode = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelOpStatus = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelOprTypeByMode = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelMajorOpByMode = new MinifilterLogModel(this);
    //File Stats
    Q_INVOKABLE MinifilterLogModel *modelFileCounts = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelFileWriteTiming = new MinifilterLogModel(this);
    //Process Stats
    Q_INVOKABLE MinifilterLogModel *modelProcessCounts = new MinifilterLogModel(this);
    Q_INVOKABLE MinifilterLogModel *modelThreadBreakdown = new MinifilterLogModel(this);
};

#endif // DATAMODELREGISTRY_H
