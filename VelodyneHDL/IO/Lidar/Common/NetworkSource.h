// Copyright 2013 Velodyne Acoustics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVelodyneHDLReader.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef NETWORKSOURCE_H
#define NETWORKSOURCE_H

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include <deque>
#include <queue>

class PacketConsumer;
class PacketReceiver;
class PacketFileWriter;
/**
* \class PacketReceiver
* \brief This class is responsible for the IOService and  two PacketReceiver classes
* @param _consumer boost::shared_ptr<PacketConsumer>
* @param argLIDARPort The used port to receive the LIDAR information
* @param ForwardedLIDARPort_ The port which will receive the lidar forwarded packets
* @param ForwardedIpAddress_ The ip which will receive the forwarded packets
* @param isForwarding_ Allow the forwarding
*/
class NetworkSource
{
public:
  NetworkSource(std::shared_ptr<PacketConsumer> _consumer, int argLIDARPort,
    int ForwardedLIDARPort_, std::string ForwardedIpAddress_,
    bool isForwarding_, bool isCrashAnalysing_)
    : IsCrashAnalysing(isCrashAnalysing_)
    , IsForwarding(isForwarding_)
    , ForwardedIpAddress(ForwardedIpAddress_)
    , ForwardedLIDARPort(ForwardedLIDARPort_)
    , LIDARPort(argLIDARPort)
    , IOService()
    , Thread()
    , LIDARPortReceiver()
    , Consumer(_consumer)
    , Writer()
    , DummyWork(new boost::asio::io_service::work(this->IOService))
  {
  }

  ~NetworkSource();

  void QueuePackets(std::string* packet);

  void Start();

  void Stop();

  //! @todo currently evrything is public, but it should be private
  bool IsCrashAnalysing;
  bool IsForwarding; /*!< Allowing the forward of the packets */
  std::string ForwardedIpAddress;   /*!< Ip of the computer which will receive the forwarded packets */
  int ForwardedLIDARPort; /*!< Port address which will receive the lidar forwarded packet */
  int LIDARPort;          /*!< Listening port for LIDAR information */
  boost::asio::io_service IOService; /*!< The in/out service which will handle the Packets */
  boost::shared_ptr<boost::thread> Thread;

  boost::shared_ptr<PacketReceiver>
    LIDARPortReceiver; /*!< The PacketReceiver configured to receive LIDAR information */

  std::shared_ptr<PacketConsumer> Consumer;
  std::shared_ptr<PacketFileWriter> Writer;

  boost::asio::io_service::work* DummyWork;
};


#endif // NETWORKSOURCE_H