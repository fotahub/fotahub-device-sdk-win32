/*
 *  Copyright (C) 2022 FotaHub Inc. All rights reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of the FotaHub(R) Device SDK program (https://fotahub.com)
 */
#include "AbstractAWSIoTMQTTThingShadowImpl.h"

#include <string.h>
#include <stdlib.h>
#include "AWSIoTMQTTThingShadowHelper.h"
#include "PhysicalUnitConversions.h"

AWSIoTSessionData_t *AbstractAWSIoTMQTTThingShadowImpl_newAWSIoTData(void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  for ( int16_t __i = 0 ; __i < MAX_AWS_IOT_MQTT_REQUEST_COUNT; __i++ )
  {
    if (!(___cid->awsIoTRequests__field[__i].busy)) 
    {
      ___cid->awsIoTRequests__field[__i].busy = true;
      ___cid->awsIoTRequests__field[__i].hDownstreamSession = NULL;
      ___cid->awsIoTRequests__field[__i].pUserData = NULL;
      ___cid->awsIoTRequests__field[__i].pPublishMessage = NULL;
      ___cid->awsIoTRequests__field[__i].pConnectionParams = NULL;
      ___cid->awsIoTRequests__field[__i].action = IOT_ACTION_INTERNAL;
      return &___cid->awsIoTRequests__field[__i];
    }
  }
  return NULL;
}

bool AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(AWSIoTSessionData_t *pSessionData, void *___id)
{
  if (pSessionData == NULL) 
  {
    return true;
  }
  return !(pSessionData->busy);
}

void AbstractAWSIoTMQTTThingShadowImpl_deleteAWSIoTData(AWSIoTSessionData_t *pSessionData, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  if (pSessionData->pPublishMessage != NULL) 
  {
    (*___cid->mqttClient__ops->deleteDatagram)(NULL, pSessionData->pPublishMessage, ___cid->mqttClient__ops->__instance);
  }
  pSessionData->busy = false;
}

bool AbstractAWSIoTMQTTThingShadowImpl_thingShadow_report(void const* hSession, char *jsonString, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *) hSession);
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return false;
  }
  if (!AbstractAWSIoTMQTTThingShadowImpl_thingShadow_isConnected(pSessionData, ___cid)) 
  {
    return false;
  }
  if (jsonString == NULL) 
  {
    return false;
  }
  
  Datagram_t *mqttPublishMessage = (*___cid->mqttClient__ops->newPublishMessage)(pSessionData->hDownstreamSession, strlen(jsonString), ___cid->mqttClient__ops->__instance);
  if (mqttPublishMessage == NULL) 
  {
    return false;
  }
  memcpy(((char *)(mqttPublishMessage->pVisiblePayload)), jsonString, strlen(jsonString));
  
  pSessionData->action = IOT_ACTION_REPORT;
  char updateTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  buildAWSIoTThingShadowTopicName(updateTopicName, pSessionData->pConnectionParams->deviceName, pSessionData->action, IOT_SUBTOPIC_KIND_NONE);
  
  MQTTPublishParameters_t mqttPubParams = 
  {
    .qos = ((pSessionData->pConnectionParams->qos == IOT_QOS_0)) ? (MQTT_QOS_0) : (MQTT_QOS_1), 
    .isRetained = false, 
    .isDuplicatedMsg = false
  };
  if (!(*___cid->mqttClient__ops->sendPublishMessage)(pSessionData->hDownstreamSession, updateTopicName, mqttPublishMessage, &mqttPubParams, ___cid->mqttClient__ops->__instance)) 
  {
    (*___cid->mqttClient__ops->deleteDatagram)(pSessionData, mqttPublishMessage, ___cid->mqttClient__ops->__instance);
    return false;
  }
  if (___cid->timeoutTimer__ops != NULL) 
  {
    (*___cid->timeoutTimer__ops->set)(((uint32_t)(s_to_ms_any(((int16_t)(pSessionData->pConnectionParams->timeout))))), false, ___cid->timeoutTimer__ops->__instance);
  }
  return true;
}

bool AbstractAWSIoTMQTTThingShadowImpl_thingShadow_get(void const* hSession, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *) hSession);
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return false;
  }
  
  if (!AbstractAWSIoTMQTTThingShadowImpl_thingShadow_isConnected(pSessionData, ___cid)) 
  {
    return false;
  }
  
  pSessionData->action = IOT_ACTION_GET;
  
  Datagram_t *mqttPublishMessage = (*___cid->mqttClient__ops->newPublishMessage)(pSessionData, 0, ___cid->mqttClient__ops->__instance);
  if (mqttPublishMessage == NULL) 
  {
    return false;
  }
  
  char getTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  buildAWSIoTThingShadowTopicName(getTopicName, pSessionData->pConnectionParams->deviceName, pSessionData->action, IOT_SUBTOPIC_KIND_NONE);
  
  MQTTPublishParameters_t mqttPubParams = 
  {
    .qos = MQTT_QOS_0, 
    .isRetained = false, 
    .isDuplicatedMsg = false
  };
  if (!(*___cid->mqttClient__ops->sendPublishMessage)(pSessionData, "$iothub/twin/GET/?$rid=get_twin", mqttPublishMessage, &mqttPubParams, ___cid->mqttClient__ops->__instance)) 
  {
    (*___cid->mqttClient__ops->deleteDatagram)(pSessionData, mqttPublishMessage, ___cid->mqttClient__ops->__instance);
    return false;
  }
  
  if (___cid->timeoutTimer__ops != NULL) 
  {
    (*___cid->timeoutTimer__ops->set)(((uint32_t)(s_to_ms_any(((int16_t)(pSessionData->pConnectionParams->timeout))))), false, ___cid->timeoutTimer__ops->__instance);
  }
  return true;
}

bool AbstractAWSIoTMQTTThingShadowImpl_thingShadow_isConnected(void const* hSession, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *) hSession);
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return false;
  }
  
  return (*___cid->mqttClient__ops->isConnected)(pSessionData->hDownstreamSession, ___cid->mqttClient__ops->__instance);
}

void AbstractAWSIoTMQTTThingShadowImpl_thingShadow_disconnect(void const* hSession, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *) hSession);
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  (*___cid->mqttClient__ops->disconnect)(pSessionData->hDownstreamSession, ___cid->mqttClient__ops->__instance);
}

void *AbstractAWSIoTMQTTThingShadowImpl_thingShadow_getUserData(void const* hSession, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *) hSession);
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return NULL;
  }
  return pSessionData->pUserData;
}

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_connected(void const* hSession, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *)((*___cid->mqttClient__ops->getUserData)(hSession, ___cid->mqttClient__ops->__instance)));
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  printf("Subscribing to AWS IoT Thing Shadow topics...\n");
  
  char updateAcceptedTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  char updateRejectedTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  char updateDeltaTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  char getAcceptedTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  char getRejectedTopicName[MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH + 1] = { 0 };
  
  buildAWSIoTThingShadowTopicName(updateAcceptedTopicName, pSessionData->pConnectionParams->deviceName, IOT_ACTION_REPORT, IOT_SUBTOPIC_KIND_ACCEPTED);
  buildAWSIoTThingShadowTopicName(updateRejectedTopicName, pSessionData->pConnectionParams->deviceName, IOT_ACTION_REPORT, IOT_SUBTOPIC_KIND_REJECTED);
  buildAWSIoTThingShadowTopicName(updateDeltaTopicName, pSessionData->pConnectionParams->deviceName, IOT_ACTION_REPORT, IOT_SUBTOPIC_KIND_DELTA);
  buildAWSIoTThingShadowTopicName(getAcceptedTopicName, pSessionData->pConnectionParams->deviceName, IOT_ACTION_GET, IOT_SUBTOPIC_KIND_ACCEPTED);
  buildAWSIoTThingShadowTopicName(getRejectedTopicName, pSessionData->pConnectionParams->deviceName, IOT_ACTION_GET, IOT_SUBTOPIC_KIND_REJECTED);
  char *topicList[AWS_IOT_THING_SHADOW_SUBTOPICS_COUNT] = { updateDeltaTopicName, updateAcceptedTopicName, updateRejectedTopicName, getAcceptedTopicName, getRejectedTopicName };
  
  MQTTQoS_t qosList[AWS_IOT_THING_SHADOW_SUBTOPICS_COUNT] = { MQTT_QOS_0, MQTT_QOS_0, MQTT_QOS_0, MQTT_QOS_0, MQTT_QOS_0 };
  
  if (!(*___cid->mqttClient__ops->subscribe)(hSession, topicList, qosList, AWS_IOT_THING_SHADOW_SUBTOPICS_COUNT, 0, false, ___cid->mqttClient__ops->__instance)) 
  {
    for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->thingShadowHandlers__ops[___pc]->connectionError)(pSessionData, IOT_ERROR_MQTT_TOPIC_SUBSCRIPTION, ___cid->thingShadowHandlers__ops[___pc]->__instance);
      }
    }
  }
}

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_subscribed(void const* hSession, uint16_t subMsgId, MQTTQoS_t grantedQoS[], void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *)((*___cid->mqttClient__ops->getUserData)(hSession, ___cid->mqttClient__ops->__instance)));
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  for ( int16_t __i = 0 ; __i < AWS_IOT_THING_SHADOW_SUBTOPICS_COUNT; __i++ )
  {
    if (grantedQoS[__i] == MQTT_QOS_FAILURE) 
    {
      for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
      {
        if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
        {
          (*___cid->thingShadowHandlers__ops[___pc]->connectionError)(pSessionData, IOT_ERROR_MQTT_TOPIC_SUBSCRIPTION, ___cid->thingShadowHandlers__ops[___pc]->__instance);
        }
      }
      return;
    }
  }
  for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
  {
    if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
    {
      (*___cid->thingShadowHandlers__ops[___pc]->connected)(pSessionData, ___cid->thingShadowHandlers__ops[___pc]->__instance);
    }
  }
}

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_published(void const* hSession, void *___id)
{
  
}

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_publishMessageReceived(void const* hSession, char *topicName, size_t topicNameLen, Datagram_t *pDatagram, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *)((*___cid->mqttClient__ops->getUserData)(hSession, ___cid->mqttClient__ops->__instance)));
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  if (___cid->timeoutTimer__ops != NULL) 
  {
    /* 
     * cancel timeout timer
     */
    (*___cid->timeoutTimer__ops->cancel)(___cid->timeoutTimer__ops->__instance);
  }
  
  IoTAction_t thingShadowAction = getAWSIoTThingShadowActionfromTopicName(topicName, topicNameLen);
  IoTSubTopicKind_t thingShadowSubTopic = getAWSIoTThingShadowSubTopicfromTopicName(topicName, topicNameLen);
  
  if ((thingShadowAction == IOT_ACTION_INTERNAL) || (thingShadowSubTopic == IOT_SUBTOPIC_KIND_NONE)) 
  {
    for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->thingShadowHandlers__ops[___pc]->connectionError)(pSessionData, IOT_ERROR_MQTT_TOPIC_SUBSCRIPTION, ___cid->thingShadowHandlers__ops[___pc]->__instance);
      }
    }
    return;
  }
  
  if (thingShadowSubTopic == IOT_SUBTOPIC_KIND_DELTA) 
  {
    for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->thingShadowHandlers__ops[___pc]->desired)(pSessionData, pSessionData->pConnectionParams->deviceName, ((char *)(pDatagram->pVisiblePayload)), ((uint32_t)(pDatagram->visiblePayloadLength)), ___cid->thingShadowHandlers__ops[___pc]->__instance);
      }
    }
  }
  else
  {
    for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->thingShadowHandlers__ops[___pc]->status)(pSessionData, pSessionData->pConnectionParams->deviceName, thingShadowAction, subTopicToAWSIoTThingShadowAckStatus(thingShadowSubTopic), ((char *)(pDatagram->pVisiblePayload)), pDatagram->visiblePayloadLength, ___cid->thingShadowHandlers__ops[___pc]->__instance);
      }
    }
  }
}

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_disconnected(void const* hSession, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *)((*___cid->mqttClient__ops->getUserData)(hSession, ___cid->mqttClient__ops->__instance)));
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
  {
    if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
    {
      (*___cid->thingShadowHandlers__ops[___pc]->disconnected)(pSessionData, ___cid->thingShadowHandlers__ops[___pc]->__instance);
    }
  }
  AbstractAWSIoTMQTTThingShadowImpl_deleteAWSIoTData(pSessionData, ___cid);
}

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_connectionError(void const* hSession, MQTTError_t error, void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  AWSIoTSessionData_t *pSessionData = ((AWSIoTSessionData_t *)((*___cid->mqttClient__ops->getUserData)(hSession, ___cid->mqttClient__ops->__instance)));
  if (AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  /* 
   * !! Important Note !! Check connection status PRIOR to calling connection error handler because upstream application
   * might decide to disconnect
   */
  bool wasConnected = (*___cid->mqttClient__ops->isConnected)(pSessionData->hDownstreamSession, ___cid->mqttClient__ops->__instance);
  if (error == MQTT_ERROR_SOCKET_CONNECTION_FAILED) 
  {
    for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->thingShadowHandlers__ops[___pc]->connectionError)(pSessionData, IOT_ERROR_NET_CONNECT_FAILED, ___cid->thingShadowHandlers__ops[___pc]->__instance);
      }
    }
  }
  else
  {
    for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->thingShadowHandlers__ops[___pc]->connectionError)(pSessionData, IOT_ERROR_MQTT_CONNECTION, ___cid->thingShadowHandlers__ops[___pc]->__instance);
      }
    }
  }
  
  /* 
   * Remove connection data only when attempt to establish a new connection has failed. In all other cases, 
   * mqttClientHandler_disconnected() will do so
   */
  if (!wasConnected) 
  {
    AbstractAWSIoTMQTTThingShadowImpl_deleteAWSIoTData(pSessionData, ___cid);
  }
}

void AbstractAWSIoTMQTTThingShadowImpl_timeoutHandler_expired(void *___id)
{
  AbstractAWSIoTMQTTThingShadowImpl__cdata_t *___cid = ((AbstractAWSIoTMQTTThingShadowImpl__cdata_t *) ___id);
  for ( int16_t __i = 0 ; __i < MAX_AWS_IOT_MQTT_REQUEST_COUNT; __i++ )
  {
    if (___cid->awsIoTRequests__field[__i].busy) 
    {
      for ( uint8_t ___pc = 0 ; ___pc < MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT; ___pc++ )
      {
        if (___cid->thingShadowHandlers__ops[___pc] != NULL && ___cid->thingShadowHandlers__ops[___pc]->__instance != NULL) 
        {
          (*___cid->thingShadowHandlers__ops[___pc]->status)(&___cid->awsIoTRequests__field[__i], ___cid->awsIoTRequests__field[__i].pConnectionParams->deviceName, ___cid->awsIoTRequests__field[__i].action, IOT_RESPONSE_STATUS_TIMEOUT, NULL, 0, ___cid->thingShadowHandlers__ops[___pc]->__instance);
        }
      }
    }
  }
}
