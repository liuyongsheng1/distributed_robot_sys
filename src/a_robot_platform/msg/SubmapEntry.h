// Generated by gencpp from file a_robot_platform/SubmapEntry.msg
// DO NOT EDIT!


#ifndef A_ROBOT_PLATFORM_MESSAGE_SUBMAPENTRY_H
#define A_ROBOT_PLATFORM_MESSAGE_SUBMAPENTRY_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <geometry_msgs/Pose.h>

namespace a_robot_platform
{
template <class ContainerAllocator>
struct SubmapEntry_
{
  typedef SubmapEntry_<ContainerAllocator> Type;

  SubmapEntry_()
    : submap_version(0)
    , pose()  {
    }
  SubmapEntry_(const ContainerAllocator& _alloc)
    : submap_version(0)
    , pose(_alloc)  {
  (void)_alloc;
    }



   typedef uint32_t _submap_version_type;
  _submap_version_type submap_version;

   typedef  ::geometry_msgs::Pose_<ContainerAllocator>  _pose_type;
  _pose_type pose;




  typedef boost::shared_ptr< ::a_robot_platform::SubmapEntry_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::a_robot_platform::SubmapEntry_<ContainerAllocator> const> ConstPtr;

}; // struct SubmapEntry_

typedef ::a_robot_platform::SubmapEntry_<std::allocator<void> > SubmapEntry;

typedef boost::shared_ptr< ::a_robot_platform::SubmapEntry > SubmapEntryPtr;
typedef boost::shared_ptr< ::a_robot_platform::SubmapEntry const> SubmapEntryConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::a_robot_platform::SubmapEntry_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace a_robot_platform

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'a_robot_platform': ['/home/zw/distributed_robot_sys/src/a_robot_platform/msg'], 'geometry_msgs': ['/opt/ros/indigo/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::a_robot_platform::SubmapEntry_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::a_robot_platform::SubmapEntry_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::a_robot_platform::SubmapEntry_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bfefc47ae367754c790b0eb0b7771be7";
  }

  static const char* value(const ::a_robot_platform::SubmapEntry_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xbfefc47ae367754cULL;
  static const uint64_t static_value2 = 0x790b0eb0b7771be7ULL;
};

template<class ContainerAllocator>
struct DataType< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
{
  static const char* value()
  {
    return "a_robot_platform/SubmapEntry";
  }

  static const char* value(const ::a_robot_platform::SubmapEntry_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# Copyright 2016 The Cartographer Authors\n\
#\n\
# Licensed under the Apache License, Version 2.0 (the \"License\");\n\
# you may not use this file except in compliance with the License.\n\
# You may obtain a copy of the License at\n\
#\n\
#      http://www.apache.org/licenses/LICENSE-2.0\n\
#\n\
# Unless required by applicable law or agreed to in writing, software\n\
# distributed under the License is distributed on an \"AS IS\" BASIS,\n\
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n\
# See the License for the specific language governing permissions and\n\
# limitations under the License.\n\
\n\
uint32 submap_version\n\
geometry_msgs/Pose pose\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Pose\n\
# A representation of pose in free space, composed of postion and orientation. \n\
Point position\n\
Quaternion orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
";
  }

  static const char* value(const ::a_robot_platform::SubmapEntry_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.submap_version);
      stream.next(m.pose);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct SubmapEntry_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::a_robot_platform::SubmapEntry_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::a_robot_platform::SubmapEntry_<ContainerAllocator>& v)
  {
    s << indent << "submap_version: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.submap_version);
    s << indent << "pose: ";
    s << std::endl;
    Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "  ", v.pose);
  }
};

} // namespace message_operations
} // namespace ros

#endif // A_ROBOT_PLATFORM_MESSAGE_SUBMAPENTRY_H
