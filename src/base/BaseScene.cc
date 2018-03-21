/*
 * Copyright (C) 2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <sstream>

#include <ignition/math/Helpers.hh>

#include <ignition/common/Console.hh>
#include <ignition/common/Mesh.hh>
#include <ignition/common/MeshManager.hh>

#include "ignition/common/Time.hh"

#include "ignition/rendering/ArrowVisual.hh"
#include "ignition/rendering/AxisVisual.hh"
#include "ignition/rendering/Camera.hh"
#include "ignition/rendering/Visual.hh"
#include "ignition/rendering/base/BaseStorage.hh"
#include "ignition/rendering/base/BaseScene.hh"

using namespace ignition;
using namespace rendering;

//////////////////////////////////////////////////
BaseScene::BaseScene(unsigned int _id, const std::string &_name) :
  id(_id),
  name(_name),
  loaded(false),
  initialized(false),
  nextObjectId(ignition::math::MAX_UI16),
  nodes(nullptr)
{
}

//////////////////////////////////////////////////
BaseScene::~BaseScene()
{
}

//////////////////////////////////////////////////
void BaseScene::Load()
{
  if (!this->loaded)
  {
    this->loaded = this->LoadImpl();
  }
}

//////////////////////////////////////////////////
void BaseScene::Init()
{
  if (!this->loaded)
  {
    ignerr << "Scene must be loaded first" << std::endl;
    return;
  }

  if (!this->initialized)
  {
    this->initialized = this->InitImpl();
    this->CreateNodeStore();
    this->CreateMaterials();
  }
}

//////////////////////////////////////////////////
void BaseScene::Fini()
{
}

//////////////////////////////////////////////////
bool BaseScene::IsLoaded() const
{
  return this->loaded;
}

//////////////////////////////////////////////////
bool BaseScene::IsInitialized() const
{
  return this->initialized;
}

//////////////////////////////////////////////////
unsigned int BaseScene::Id() const
{
  return this->id;
}

//////////////////////////////////////////////////
std::string BaseScene::Name() const
{
  return this->name;
}

//////////////////////////////////////////////////
common::Time BaseScene::SimTime() const
{
  return this->simTime;
}

//////////////////////////////////////////////////
void BaseScene::SetSimTime(const common::Time &_time)
{
  this->simTime = _time;
}

//////////////////////////////////////////////////
VisualPtr BaseScene::VisualAt(const CameraPtr &_camera,
                              const ignition::math::Vector2i &_mousePos)
{
  return this->VisualAt(_camera, _mousePos);
}

//////////////////////////////////////////////////
void BaseScene::SetAmbientLight(double _r, double _g, double _b, double _a)
{
  this->SetAmbientLight(math::Color(_r, _g, _b, _a));
}

//////////////////////////////////////////////////
math::Color BaseScene::BackgroundColor() const
{
  return this->backgroundColor;
}

//////////////////////////////////////////////////
void BaseScene::SetBackgroundColor(double _r, double _g, double _b, double _a)
{
  this->SetBackgroundColor(math::Color(_r, _g, _b, _a));
}

//////////////////////////////////////////////////
void BaseScene::SetBackgroundColor(const math::Color &_color)
{
  this->backgroundColor = _color;
}

//////////////////////////////////////////////////
unsigned int BaseScene::NodeCount() const
{
  return this->nodes->Size();
}

//////////////////////////////////////////////////
bool BaseScene::HasNode(ConstNodePtr _node) const
{
  return this->nodes->Contains(_node);
}

//////////////////////////////////////////////////
bool BaseScene::HasNodeId(unsigned int _id) const
{
  return this->nodes->ContainsId(_id);
}

//////////////////////////////////////////////////
bool BaseScene::HasNodeName(const std::string &_name) const
{
  return this->nodes->ContainsName(_name);
}

//////////////////////////////////////////////////
NodePtr BaseScene::NodeById(unsigned int _id) const
{
  return this->nodes->GetById(_id);
}

//////////////////////////////////////////////////
NodePtr BaseScene::NodeByName(const std::string &_name) const
{
  return this->nodes->GetByName(_name);
}

//////////////////////////////////////////////////
NodePtr BaseScene::NodeByIndex(unsigned int _index) const
{
  return this->nodes->GetByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroyNode(NodePtr _node)
{
  this->nodes->Destroy(_node);
}

//////////////////////////////////////////////////
void BaseScene::DestroyNodeById(unsigned int _id)
{
  this->nodes->DestroyById(_id);
}

//////////////////////////////////////////////////
void BaseScene::DestroyNodeByName(const std::string &_name)
{
  this->nodes->DestroyByName(_name);
}

//////////////////////////////////////////////////
void BaseScene::DestroyNodeByIndex(unsigned int _index)
{
  this->nodes->DestroyByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroyNodes()
{
  this->nodes->DestroyAll();
}

//////////////////////////////////////////////////
unsigned int BaseScene::LightCount() const
{
  return this->Lights()->Size();
}

//////////////////////////////////////////////////
bool BaseScene::HasLight(ConstLightPtr _light) const
{
  return this->Lights()->Contains(_light);
}

//////////////////////////////////////////////////
bool BaseScene::HasLightId(unsigned int _id) const
{
  return this->Lights()->ContainsId(_id);
}

//////////////////////////////////////////////////
bool BaseScene::HasLightName(const std::string &_name) const
{
  return this->Lights()->ContainsName(_name);
}

//////////////////////////////////////////////////
LightPtr BaseScene::LightById(unsigned int _id) const
{
  return this->Lights()->GetById(_id);
}

//////////////////////////////////////////////////
LightPtr BaseScene::LightByName(const std::string &_name) const
{
  return this->Lights()->GetByName(_name);
}

//////////////////////////////////////////////////
LightPtr BaseScene::LightByIndex(unsigned int _index) const
{
  return this->Lights()->GetByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroyLight(LightPtr _light)
{
  this->Lights()->Destroy(_light);
}

//////////////////////////////////////////////////
void BaseScene::DestroyLightById(unsigned int _id)
{
  this->Lights()->DestroyById(_id);
}

//////////////////////////////////////////////////
void BaseScene::DestroyLightByName(const std::string &_name)
{
  this->Lights()->DestroyByName(_name);
}

//////////////////////////////////////////////////
void BaseScene::DestroyLightByIndex(unsigned int _index)
{
  this->Lights()->DestroyByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroyLights()
{
  this->Lights()->DestroyAll();
}

//////////////////////////////////////////////////
unsigned int BaseScene::SensorCount() const
{
  return this->Sensors()->Size();
}

//////////////////////////////////////////////////
bool BaseScene::HasSensor(ConstSensorPtr _sensor) const
{
  return this->Sensors()->Contains(_sensor);
}

//////////////////////////////////////////////////
bool BaseScene::HasSensorId(unsigned int _id) const
{
  return this->Sensors()->ContainsId(_id);
}

//////////////////////////////////////////////////
bool BaseScene::HasSensorName(const std::string &_name) const
{
  return this->Sensors()->ContainsName(_name);
}

//////////////////////////////////////////////////
SensorPtr BaseScene::SensorById(unsigned int _id) const
{
  return this->Sensors()->GetById(_id);
}

//////////////////////////////////////////////////
SensorPtr BaseScene::SensorByName(const std::string &_name) const
{
  return this->Sensors()->GetByName(_name);
}

//////////////////////////////////////////////////
SensorPtr BaseScene::SensorByIndex(unsigned int _index) const
{
  return this->Sensors()->GetByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroySensor(SensorPtr _sensor)
{
  this->Sensors()->Destroy(_sensor);
}

//////////////////////////////////////////////////
void BaseScene::DestroySensorById(unsigned int _id)
{
  this->Sensors()->DestroyById(_id);
}

//////////////////////////////////////////////////
void BaseScene::DestroySensorByName(const std::string &_name)
{
  this->Sensors()->DestroyByName(_name);
}

//////////////////////////////////////////////////
void BaseScene::DestroySensorByIndex(unsigned int _index)
{
  this->Sensors()->DestroyByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroySensors()
{
  this->Sensors()->DestroyAll();
}

//////////////////////////////////////////////////
unsigned int BaseScene::VisualCount() const
{
  return this->Visuals()->Size();
}

//////////////////////////////////////////////////
bool BaseScene::HasVisual(ConstVisualPtr _visual) const
{
  return this->Visuals()->Contains(_visual);
}

//////////////////////////////////////////////////
bool BaseScene::HasVisualId(unsigned int _id) const
{
  return this->Visuals()->ContainsId(_id);
}

//////////////////////////////////////////////////
bool BaseScene::HasVisualName(const std::string &_name) const
{
  return this->Visuals()->ContainsName(_name);
}

//////////////////////////////////////////////////
VisualPtr BaseScene::VisualById(unsigned int _id) const
{
  return this->Visuals()->GetById(_id);
}

//////////////////////////////////////////////////
VisualPtr BaseScene::VisualByName(const std::string &_name) const
{
  return this->Visuals()->GetByName(_name);
}

//////////////////////////////////////////////////
VisualPtr BaseScene::VisualByIndex(unsigned int _index) const
{
  return this->Visuals()->GetByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroyVisual(VisualPtr _visual)
{
  this->Visuals()->Destroy(_visual);
}

//////////////////////////////////////////////////
void BaseScene::DestroyVisualById(unsigned int _id)
{
  this->Visuals()->DestroyById(_id);
}

//////////////////////////////////////////////////
void BaseScene::DestroyVisualByName(const std::string &_name)
{
  this->Visuals()->DestroyByName(_name);
}

//////////////////////////////////////////////////
void BaseScene::DestroyVisualByIndex(unsigned int _index)
{
  this->Visuals()->DestroyByIndex(_index);
}

//////////////////////////////////////////////////
void BaseScene::DestroyVisuals()
{
  this->Visuals()->DestroyAll();
}

//////////////////////////////////////////////////
bool BaseScene::MaterialRegistered(const std::string &_name) const
{
  return this->Materials()->ContainsKey(_name);
}

//////////////////////////////////////////////////
MaterialPtr BaseScene::Material(const std::string &_name) const
{
  return this->Materials()->Get(_name);
}

//////////////////////////////////////////////////
void BaseScene::RegisterMaterial(const std::string &_name,
    MaterialPtr _material)
{
  if (_material)
    this->Materials()->Put(_name, _material);
}

//////////////////////////////////////////////////
void BaseScene::UnregisterMaterial(const std::string &_name)
{
  this->Materials()->Remove(_name);
}

//////////////////////////////////////////////////
void BaseScene::UnregisterMaterials()
{
  this->Materials()->RemoveAll();
}

//////////////////////////////////////////////////
DirectionalLightPtr BaseScene::CreateDirectionalLight()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateDirectionalLight(objId);
}

//////////////////////////////////////////////////
DirectionalLightPtr BaseScene::CreateDirectionalLight(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "DirectionalLight");
  return this->CreateDirectionalLight(_id, objName);
}

//////////////////////////////////////////////////
DirectionalLightPtr BaseScene::CreateDirectionalLight(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateDirectionalLight(objId, _name);
}

//////////////////////////////////////////////////
DirectionalLightPtr BaseScene::CreateDirectionalLight(unsigned int _id,
    const std::string &_name)
{
  DirectionalLightPtr light = this->CreateDirectionalLightImpl(_id, _name);
  bool result = this->RegisterLight(light);
  return (result) ? light : nullptr;
}

//////////////////////////////////////////////////
PointLightPtr BaseScene::CreatePointLight()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreatePointLight(objId);
}
//////////////////////////////////////////////////
PointLightPtr BaseScene::CreatePointLight(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "PointLight");
  return this->CreatePointLight(_id, objName);
}
//////////////////////////////////////////////////
PointLightPtr BaseScene::CreatePointLight(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreatePointLight(objId, _name);
}
//////////////////////////////////////////////////
PointLightPtr BaseScene::CreatePointLight(unsigned int _id,
    const std::string &_name)
{
  PointLightPtr light = this->CreatePointLightImpl(_id, _name);
  bool result = this->RegisterLight(light);
  return (result) ? light : nullptr;
}

//////////////////////////////////////////////////
SpotLightPtr BaseScene::CreateSpotLight()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateSpotLight(objId);
}
//////////////////////////////////////////////////
SpotLightPtr BaseScene::CreateSpotLight(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "SpotLight");
  return this->CreateSpotLight(_id, objName);
}
//////////////////////////////////////////////////
SpotLightPtr BaseScene::CreateSpotLight(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateSpotLight(objId, _name);
}
//////////////////////////////////////////////////
SpotLightPtr BaseScene::CreateSpotLight(unsigned int _id,
    const std::string &_name)
{
  SpotLightPtr light = this->CreateSpotLightImpl(_id, _name);
  bool result = this->RegisterLight(light);
  return (result) ? light : nullptr;
}

//////////////////////////////////////////////////
CameraPtr BaseScene::CreateCamera()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateCamera(objId);
}
//////////////////////////////////////////////////
CameraPtr BaseScene::CreateCamera(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "Camera");
  return this->CreateCamera(_id, objName);
}
//////////////////////////////////////////////////
CameraPtr BaseScene::CreateCamera(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateCamera(objId, _name);
}
//////////////////////////////////////////////////
CameraPtr BaseScene::CreateCamera(unsigned int _id, const std::string &_name)
{
  CameraPtr camera = this->CreateCameraImpl(_id, _name);
  bool result = this->RegisterSensor(camera);
  return (result) ? camera : nullptr;
}

//////////////////////////////////////////////////
VisualPtr BaseScene::CreateVisual()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateVisual(objId);
}
//////////////////////////////////////////////////
VisualPtr BaseScene::CreateVisual(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "Visual");
  return this->CreateVisual(_id, objName);
}
//////////////////////////////////////////////////
VisualPtr BaseScene::CreateVisual(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateVisual(objId, _name);
}
//////////////////////////////////////////////////
VisualPtr BaseScene::CreateVisual(unsigned int _id, const std::string &_name)
{
  VisualPtr visual = this->CreateVisualImpl(_id, _name);
  bool result = this->RegisterVisual(visual);
  return (result) ? visual : nullptr;
}

//////////////////////////////////////////////////
ArrowVisualPtr BaseScene::CreateArrowVisual()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateArrowVisual(objId);
}
//////////////////////////////////////////////////
ArrowVisualPtr BaseScene::CreateArrowVisual(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "ArrowVisual");
  return this->CreateArrowVisual(_id, objName);
}
//////////////////////////////////////////////////
ArrowVisualPtr BaseScene::CreateArrowVisual(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateArrowVisual(objId, _name);
}
//////////////////////////////////////////////////
ArrowVisualPtr BaseScene::CreateArrowVisual(unsigned int _id,
    const std::string &_name)
{
  ArrowVisualPtr visual = this->CreateArrowVisualImpl(_id, _name);
  bool result = this->RegisterVisual(visual);
  return (result) ? visual : nullptr;
}

//////////////////////////////////////////////////
AxisVisualPtr BaseScene::CreateAxisVisual()
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateAxisVisual(objId);
}
//////////////////////////////////////////////////
AxisVisualPtr BaseScene::CreateAxisVisual(unsigned int _id)
{
  std::string objName = this->CreateObjectName(_id, "AxisVisual");
  return this->CreateAxisVisual(_id, objName);
}
//////////////////////////////////////////////////
AxisVisualPtr BaseScene::CreateAxisVisual(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();
  return this->CreateAxisVisual(objId, _name);
}
//////////////////////////////////////////////////
AxisVisualPtr BaseScene::CreateAxisVisual(unsigned int _id,
    const std::string &_name)
{
  AxisVisualPtr visual = this->CreateAxisVisualImpl(_id, _name);
  bool result = this->RegisterVisual(visual);
  return (result) ? visual : nullptr;
}

//////////////////////////////////////////////////
GeometryPtr BaseScene::CreateBox()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Box");
  return this->CreateBoxImpl(objId, objName);
}

//////////////////////////////////////////////////
GeometryPtr BaseScene::CreateCone()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Cone");
  return this->CreateConeImpl(objId, objName);
}

//////////////////////////////////////////////////
GeometryPtr BaseScene::CreateCylinder()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Cylinder");
  return this->CreateCylinderImpl(objId, objName);
}

//////////////////////////////////////////////////
GeometryPtr BaseScene::CreatePlane()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Plane");
  return this->CreatePlaneImpl(objId, objName);
}

//////////////////////////////////////////////////
GeometryPtr BaseScene::CreateSphere()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Sphere");
  return this->CreateSphereImpl(objId, objName);
}

//////////////////////////////////////////////////
MeshPtr BaseScene::CreateMesh(const std::string &_meshName)
{
  MeshDescriptor descriptor(_meshName);
  return this->CreateMesh(descriptor);
}

//////////////////////////////////////////////////
MeshPtr BaseScene::CreateMesh(const common::Mesh *_mesh)
{
  MeshDescriptor descriptor(_mesh);
  return this->CreateMesh(descriptor);
}

//////////////////////////////////////////////////
MeshPtr BaseScene::CreateMesh(const MeshDescriptor &_desc)
{
  std::string meshName = (_desc.mesh) ?
      _desc.mesh->Name() : _desc.meshName;

  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Mesh-" + meshName);
  return this->CreateMeshImpl(objId, objName, _desc);
}

//////////////////////////////////////////////////
GridPtr BaseScene::CreateGrid()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "Grid");
  return this->CreateGridImpl(objId, objName);
}

//////////////////////////////////////////////////
MaterialPtr BaseScene::CreateMaterial(const std::string &_name)
{
  unsigned int objId = this->CreateObjectId();

  std::string objName = _name.empty() ?
      this->CreateObjectName(objId, "Material") : _name;

  MaterialPtr material = this->CreateMaterialImpl(objId, objName);
  this->RegisterMaterial(objName, material);

  return material;
}

//////////////////////////////////////////////////
MaterialPtr BaseScene::CreateMaterial(const common::Material &_material)
{
  MaterialPtr material;
  unsigned int objId = this->CreateObjectId();
  std::string objName = _material.Name().empty() ?
      this->CreateObjectName(objId, "Material") : _material.Name();

  material = this->CreateMaterialImpl(objId, objName);
  material->CopyFrom(_material);
  this->RegisterMaterial(objName, material);

  return material;
}

//////////////////////////////////////////////////
RenderTexturePtr BaseScene::CreateRenderTexture()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "RenderTexture");
  return this->CreateRenderTextureImpl(objId, objName);
}

//////////////////////////////////////////////////
RenderWindowPtr BaseScene::CreateRenderWindow()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "RenderWindow");
  return this->CreateRenderWindowImpl(objId, objName);
}

//////////////////////////////////////////////////
RayQueryPtr BaseScene::CreateRayQuery()
{
  unsigned int objId = this->CreateObjectId();
  std::string objName = this->CreateObjectName(objId, "RayQuery");
  return this->CreateRayQueryImpl(objId, objName);
}

//////////////////////////////////////////////////
void BaseScene::PreRender()
{
  this->RootVisual()->PreRender();
}

//////////////////////////////////////////////////
void BaseScene::Clear()
{
  this->nodes->DestroyAll();
  this->Materials()->RemoveAll();
  this->nextObjectId = ignition::math::MAX_UI16;
}

//////////////////////////////////////////////////
void BaseScene::Destroy()
{
  // TODO: destroy context
  this->Clear();
}

//////////////////////////////////////////////////
unsigned int BaseScene::CreateObjectId()
{
  return this->nextObjectId--;
}

//////////////////////////////////////////////////
std::string BaseScene::CreateObjectName(unsigned int _id,
    const std::string &_prefix)
{
  std::stringstream ss;
  ss << this->name << "::" << _prefix;
  ss << "(" << std::to_string(_id) << ")";
  return ss.str();
}

//////////////////////////////////////////////////
bool BaseScene::RegisterLight(LightPtr _light)
{
  return (_light) ? this->Lights()->Add(_light) : false;
}

//////////////////////////////////////////////////
bool BaseScene::RegisterSensor(SensorPtr _sensor)
{
  return (_sensor) ? this->Sensors()->Add(_sensor) : false;
}

//////////////////////////////////////////////////
bool BaseScene::RegisterVisual(VisualPtr _visual)
{
  return (_visual) ? this->Visuals()->Add(_visual) : false;
}

//////////////////////////////////////////////////
void BaseScene::CreateNodeStore()
{
  NodeCompositeStorePtr compStore(new BaseNodeCompositeStore);

  // get specific stores
  LightStorePtr lights = this->Lights();
  SensorStorePtr sensors = this->Sensors();
  VisualStorePtr visuals = this->Visuals();

  // convert to node stores
  NodeStorePtr lightNodes(new BaseStoreWrapper<Node, Light>(lights));
  NodeStorePtr sensorNodes(new BaseStoreWrapper<Node, Sensor>(sensors));
  NodeStorePtr visualNodes(new BaseStoreWrapper<Node, Visual>(visuals));

  // add to composite store
  compStore->AddStore(lightNodes);
  compStore->AddStore(sensorNodes);
  compStore->AddStore(visualNodes);

  this->nodes = compStore;
}

//////////////////////////////////////////////////
void BaseScene::CreateMaterials()
{
  MaterialPtr material;

  material = this->CreateMaterial("Default/TransRed");
  material->SetAmbient(1.0, 0.0, 0.0);
  material->SetDiffuse(1.0, 0.0, 0.0);
  material->SetEmissive(1.0, 0.0, 0.0);
  material->SetTransparency(0.5);
  material->SetCastShadows(false);
  material->SetReceiveShadows(false);
  material->SetLightingEnabled(false);

  material = this->CreateMaterial("Default/TransGreen");
  material->SetAmbient(0.0, 1.0, 0.0);
  material->SetDiffuse(0.0, 1.0, 0.0);
  material->SetEmissive(0.0, 1.0, 0.0);
  material->SetTransparency(0.5);
  material->SetCastShadows(false);
  material->SetReceiveShadows(false);
  material->SetLightingEnabled(false);

  material = this->CreateMaterial("Default/TransBlue");
  material->SetAmbient(0.0, 0.0, 1.0);
  material->SetDiffuse(0.0, 0.0, 1.0);
  material->SetEmissive(0.0, 0.0, 1.0);
  material->SetTransparency(0.5);
  material->SetCastShadows(false);
  material->SetReceiveShadows(false);
  material->SetLightingEnabled(false);

  material = this->CreateMaterial("Default/White");
  material->SetAmbient(1.0, 1.0, 1.0);
  material->SetDiffuse(1.0, 1.0, 1.0);
  material->SetEmissive(1.0, 1.0, 1.0);
  material->SetTransparency(0);
  material->SetCastShadows(true);
  material->SetReceiveShadows(true);
  material->SetLightingEnabled(true);
}
