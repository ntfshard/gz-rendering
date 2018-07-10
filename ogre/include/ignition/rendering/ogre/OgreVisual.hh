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
#ifndef IGNITION_RENDERING_OGRE_OGREVISUAL_HH_
#define IGNITION_RENDERING_OGRE_OGREVISUAL_HH_

#include "ignition/rendering/base/BaseVisual.hh"
#include "ignition/rendering/ogre/OgreNode.hh"
#include "ignition/rendering/ogre/OgreRenderTypes.hh"

namespace ignition
{
  namespace rendering
  {
    class IGNITION_RENDERING_OGRE_VISIBLE OgreVisual :
      public BaseVisual<OgreNode>
    {
      protected: OgreVisual();

      public: virtual ~OgreVisual();

      public: virtual math::Vector3d LocalScale() const;

      public: virtual bool InheritScale() const;

      public: virtual void SetInheritScale(bool _inherit);

      public: virtual void SetVisible(bool _visible);

      protected: virtual NodeStorePtr Children() const;

      protected: virtual GeometryStorePtr Geometries() const;

      protected: virtual bool AttachChild(NodePtr _child);

      protected: virtual bool DetachChild(NodePtr _child);

      protected: virtual bool AttachGeometry(GeometryPtr _geometry);

      protected: virtual bool DetachGeometry(GeometryPtr _geometry);

      protected: virtual void SetLocalScaleImpl(
                     const math::Vector3d &_scale);

      protected: virtual void Init();

      protected: OgreNodeStorePtr children;

      protected: OgreGeometryStorePtr geometries;

      private: OgreVisualPtr SharedThis();

      private: friend class OgreScene;
    };
  }
}
#endif
