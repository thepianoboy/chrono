// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban, Justin Madsen
// =============================================================================
//
// UAZBUS wheel subsystem
//
// =============================================================================

#ifndef UAZBUS_WHEEL_H
#define UAZBUS_WHEEL_H

#include "chrono/assets/ChTriangleMeshShape.h"

#include "chrono_vehicle/ChSubsysDefs.h"
#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_vehicle/wheeled_vehicle/ChWheel.h"

#include "chrono_models/ChApiModels.h"

namespace chrono {
namespace vehicle {
namespace uaz {

/// @addtogroup vehicle_models_uaz
/// @{

/// UAZBUS wheel base class.
class CH_MODELS_API UAZBUS_Wheel : public ChWheel {
  public:
    UAZBUS_Wheel(const std::string& name);
    ~UAZBUS_Wheel() {}

    virtual double GetMass() const override { return m_mass; }
    virtual ChVector<> GetInertia() const override { return m_inertia; }
    virtual double GetRadius() const override { return m_radius; }
    virtual double GetWidth() const override { return m_width; }

    virtual void AddVisualizationAssets(VisualizationType vis) override;
    virtual void RemoveVisualizationAssets() override;

  protected:
    virtual std::string GetMeshName() const = 0;
    virtual std::string GetMeshFile() const = 0;

    std::shared_ptr<ChTriangleMeshShape> m_trimesh_shape;

    static const double m_radius;
    static const double m_width;
    static const double m_mass;
    static const ChVector<> m_inertia;
};

/// UAZBUS left wheel (front or rear).
class CH_MODELS_API UAZBUS_WheelLeft : public UAZBUS_Wheel {
  public:
    UAZBUS_WheelLeft(const std::string& name);
    ~UAZBUS_WheelLeft() {}

    virtual std::string GetMeshName() const override { return m_meshName; }
    virtual std::string GetMeshFile() const override { return GetDataFile(m_meshFile); }

  private:
    static const std::string m_meshName;
    static const std::string m_meshFile;
};

/// UAZBUS right wheel (front or rear).
class CH_MODELS_API UAZBUS_WheelRight : public UAZBUS_Wheel {
  public:
    UAZBUS_WheelRight(const std::string& name);
    ~UAZBUS_WheelRight() {}

    virtual std::string GetMeshName() const override { return m_meshName; }
    virtual std::string GetMeshFile() const override { return GetDataFile(m_meshFile); }

  private:
    static const std::string m_meshName;
    static const std::string m_meshFile;
};

/// @} vehicle_models_uaz

}  // end namespace uaz
}  // end namespace vehicle
}  // end namespace chrono

#endif
