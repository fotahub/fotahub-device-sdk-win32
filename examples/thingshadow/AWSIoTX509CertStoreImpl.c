/*
 *  Copyright (C) 2020-2021 FotaHub Inc. All rights reserved.
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
#include "AWSIoTX509CertStoreImpl.h"

#include <string.h>

static char const AWS_IOT_ROOT_CA_1[] = { '-', '-', '-', '-', '-', 'B', 'E', 'G', 'I', 'N', ' ', 
'C', 'E', 'R', 'T', 'I', 'F', 'I', 'C', 'A', 'T', 'E', 
'-', '-', '-', '-', '-', '\n', 'M', 'I', 'I', 'D', 'Q', 
'T', 'C', 'C', 'A', 'i', 'm', 'g', 'A', 'w', 'I', 'B', 
'A', 'g', 'I', 'T', 'B', 'm', 'y', 'f', 'z', '5', 'm', 
'/', 'j', 'A', 'o', '5', '4', 'v', 'B', '4', 'i', 'k', 
'P', 'm', 'l', 'j', 'Z', 'b', 'y', 'j', 'A', 'N', 'B', 
'g', 'k', 'q', 'h', 'k', 'i', 'G', '9', 'w', '0', 'B', 
'A', 'Q', 's', 'F', '\n', 'A', 'D', 'A', '5', 'M', 'Q', 
's', 'w', 'C', 'Q', 'Y', 'D', 'V', 'Q', 'Q', 'G', 'E', 
'w', 'J', 'V', 'U', 'z', 'E', 'P', 'M', 'A', '0', 'G', 
'A', '1', 'U', 'E', 'C', 'h', 'M', 'G', 'Q', 'W', '1', 
'h', 'e', 'm', '9', 'u', 'M', 'R', 'k', 'w', 'F', 'w', 
'Y', 'D', 'V', 'Q', 'Q', 'D', 'E', 'x', 'B', 'B', 'b', 
'W', 'F', '6', '\n', 'b', '2', '4', 'g', 'U', 'm', '9', 
'v', 'd', 'C', 'B', 'D', 'Q', 'S', 'A', 'x', 'M', 'B', 
'4', 'X', 'D', 'T', 'E', '1', 'M', 'D', 'U', 'y', 'N', 
'j', 'A', 'w', 'M', 'D', 'A', 'w', 'M', 'F', 'o', 'X', 
'D', 'T', 'M', '4', 'M', 'D', 'E', 'x', 'N', 'z', 'A', 
'w', 'M', 'D', 'A', 'w', 'M', 'F', 'o', 'w', 'O', 'T', 
'E', 'L', '\n', 'M', 'A', 'k', 'G', 'A', '1', 'U', 'E', 
'B', 'h', 'M', 'C', 'V', 'V', 'M', 'x', 'D', 'z', 'A', 
'N', 'B', 'g', 'N', 'V', 'B', 'A', 'o', 'T', 'B', 'k', 
'F', 't', 'Y', 'X', 'p', 'v', 'b', 'j', 'E', 'Z', 'M', 
'B', 'c', 'G', 'A', '1', 'U', 'E', 'A', 'x', 'M', 'Q', 
'Q', 'W', '1', 'h', 'e', 'm', '9', 'u', 'I', 'F', 'J', 
'v', '\n', 'b', '3', 'Q', 'g', 'Q', '0', 'E', 'g', 'M', 
'T', 'C', 'C', 'A', 'S', 'I', 'w', 'D', 'Q', 'Y', 'J', 
'K', 'o', 'Z', 'I', 'h', 'v', 'c', 'N', 'A', 'Q', 'E', 
'B', 'B', 'Q', 'A', 'D', 'g', 'g', 'E', 'P', 'A', 'D', 
'C', 'C', 'A', 'Q', 'o', 'C', 'g', 'g', 'E', 'B', 'A', 
'L', 'J', '4', 'g', 'H', 'H', 'K', 'e', 'N', 'X', 'j', 
'\n', 'c', 'a', '9', 'H', 'g', 'F', 'B', '0', 'f', 'W', 
'7', 'Y', '1', '4', 'h', '2', '9', 'J', 'l', 'o', '9', 
'1', 'g', 'h', 'Y', 'P', 'l', '0', 'h', 'A', 'E', 'v', 
'r', 'A', 'I', 't', 'h', 't', 'O', 'g', 'Q', '3', 'p', 
'O', 's', 'q', 'T', 'Q', 'N', 'r', 'o', 'B', 'v', 'o', 
'3', 'b', 'S', 'M', 'g', 'H', 'F', 'z', 'Z', 'M', '\n', 
'9', 'O', '6', 'I', 'I', '8', 'c', '+', '6', 'z', 'f', 
'1', 't', 'R', 'n', '4', 'S', 'W', 'i', 'w', '3', 't', 
'e', '5', 'd', 'j', 'g', 'd', 'Y', 'Z', '6', 'k', '/', 
'o', 'I', '2', 'p', 'e', 'V', 'K', 'V', 'u', 'R', 'F', 
'4', 'f', 'n', '9', 't', 'B', 'b', '6', 'd', 'N', 'q', 
'c', 'm', 'z', 'U', '5', 'L', '/', 'q', 'w', '\n', 'I', 
'F', 'A', 'G', 'b', 'H', 'r', 'Q', 'g', 'L', 'K', 'm', 
'+', 'a', '/', 's', 'R', 'x', 'm', 'P', 'U', 'D', 'g', 
'H', '3', 'K', 'K', 'H', 'O', 'V', 'j', '4', 'u', 't', 
'W', 'p', '+', 'U', 'h', 'n', 'M', 'J', 'b', 'u', 'l', 
'H', 'h', 'e', 'b', '4', 'm', 'j', 'U', 'c', 'A', 'w', 
'h', 'm', 'a', 'h', 'R', 'W', 'a', '6', '\n', 'V', 'O', 
'u', 'j', 'w', '5', 'H', '5', 'S', 'N', 'z', '/', '0', 
'e', 'g', 'w', 'L', 'X', '0', 't', 'd', 'H', 'A', '1', 
'1', '4', 'g', 'k', '9', '5', '7', 'E', 'W', 'W', '6', 
'7', 'c', '4', 'c', 'X', '8', 'j', 'J', 'G', 'K', 'L', 
'h', 'D', '+', 'r', 'c', 'd', 'q', 's', 'q', '0', '8', 
'p', '8', 'k', 'D', 'i', '1', 'L', '\n', '9', '3', 'F', 
'c', 'X', 'm', 'n', '/', '6', 'p', 'U', 'C', 'y', 'z', 
'i', 'K', 'r', 'l', 'A', '4', 'b', '9', 'v', '7', 'L', 
'W', 'I', 'b', 'x', 'c', 'c', 'e', 'V', 'O', 'F', '3', 
'4', 'G', 'f', 'I', 'D', '5', 'y', 'H', 'I', '9', 'Y', 
'/', 'Q', 'C', 'B', '/', 'I', 'I', 'D', 'E', 'g', 'E', 
'w', '+', 'O', 'y', 'Q', 'm', '\n', 'j', 'g', 'S', 'u', 
'b', 'J', 'r', 'I', 'q', 'g', '0', 'C', 'A', 'w', 'E', 
'A', 'A', 'a', 'N', 'C', 'M', 'E', 'A', 'w', 'D', 'w', 
'Y', 'D', 'V', 'R', '0', 'T', 'A', 'Q', 'H', '/', 'B', 
'A', 'U', 'w', 'A', 'w', 'E', 'B', '/', 'z', 'A', 'O', 
'B', 'g', 'N', 'V', 'H', 'Q', '8', 'B', 'A', 'f', '8', 
'E', 'B', 'A', 'M', 'C', '\n', 'A', 'Y', 'Y', 'w', 'H', 
'Q', 'Y', 'D', 'V', 'R', '0', 'O', 'B', 'B', 'Y', 'E', 
'F', 'I', 'Q', 'Y', 'z', 'I', 'U', '0', '7', 'L', 'w', 
'M', 'l', 'J', 'Q', 'u', 'C', 'F', 'm', 'c', 'x', '7', 
'I', 'Q', 'T', 'g', 'o', 'I', 'M', 'A', '0', 'G', 'C', 
'S', 'q', 'G', 'S', 'I', 'b', '3', 'D', 'Q', 'E', 'B', 
'C', 'w', 'U', 'A', '\n', 'A', '4', 'I', 'B', 'A', 'Q', 
'C', 'Y', '8', 'j', 'd', 'a', 'Q', 'Z', 'C', 'h', 'G', 
's', 'V', '2', 'U', 'S', 'g', 'g', 'N', 'i', 'M', 'O', 
'r', 'u', 'Y', 'o', 'u', '6', 'r', '4', 'l', 'K', '5', 
'I', 'p', 'D', 'B', '/', 'G', '/', 'w', 'k', 'j', 'U', 
'u', '0', 'y', 'K', 'G', 'X', '9', 'r', 'b', 'x', 'e', 
'n', 'D', 'I', '\n', 'U', '5', 'P', 'M', 'C', 'C', 'j', 
'j', 'm', 'C', 'X', 'P', 'I', '6', 'T', '5', '3', 'i', 
'H', 'T', 'f', 'I', 'U', 'J', 'r', 'U', '6', 'a', 'd', 
'T', 'r', 'C', 'C', '2', 'q', 'J', 'e', 'H', 'Z', 'E', 
'R', 'x', 'h', 'l', 'b', 'I', '1', 'B', 'j', 'j', 't', 
'/', 'm', 's', 'v', '0', 't', 'a', 'd', 'Q', '1', 'w', 
'U', 's', '\n', 'N', '+', 'g', 'D', 'S', '6', '3', 'p', 
'Y', 'a', 'A', 'C', 'b', 'v', 'X', 'y', '8', 'M', 'W', 
'y', '7', 'V', 'u', '3', '3', 'P', 'q', 'U', 'X', 'H', 
'e', 'e', 'E', '6', 'V', '/', 'U', 'q', '2', 'V', '8', 
'v', 'i', 'T', 'O', '9', '6', 'L', 'X', 'F', 'v', 'K', 
'W', 'l', 'J', 'b', 'Y', 'K', '8', 'U', '9', '0', 'v', 
'v', '\n', 'o', '/', 'u', 'f', 'Q', 'J', 'V', 't', 'M', 
'V', 'T', '8', 'Q', 't', 'P', 'H', 'R', 'h', '8', 'j', 
'r', 'd', 'k', 'P', 'S', 'H', 'C', 'a', '2', 'X', 'V', 
'4', 'c', 'd', 'F', 'y', 'Q', 'z', 'R', '1', 'b', 'l', 
'd', 'Z', 'w', 'g', 'J', 'c', 'J', 'm', 'A', 'p', 'z', 
'y', 'M', 'Z', 'F', 'o', '6', 'I', 'Q', '6', 'X', 'U', 
'\n', '5', 'M', 's', 'I', '+', 'y', 'M', 'R', 'Q', '+', 
'h', 'D', 'K', 'X', 'J', 'i', 'o', 'a', 'l', 'd', 'X', 
'g', 'j', 'U', 'k', 'K', '6', '4', '2', 'M', '4', 'U', 
'w', 't', 'B', 'V', '8', 'o', 'b', '2', 'x', 'J', 'N', 
'D', 'd', '2', 'Z', 'h', 'w', 'L', 'n', 'o', 'Q', 'd', 
'e', 'X', 'e', 'G', 'A', 'D', 'b', 'k', 'p', 'y', '\n', 
'r', 'q', 'X', 'R', 'f', 'b', 'o', 'Q', 'n', 'o', 'Z', 
's', 'G', '4', 'q', '5', 'W', 'T', 'P', '4', '6', '8', 
'S', 'Q', 'v', 'v', 'G', '5', '\n', '-', '-', '-', '-', 
'-', 'E', 'N', 'D', ' ', 'C', 'E', 'R', 'T', 'I', 'F', 
'I', 'C', 'A', 'T', 'E', '-', '-', '-', '-', '-', '\0'
 };

static char const AWS_IOT_ROOT_CA_3[] = { '-', '-', '-', '-', '-', 'B', 'E', 'G', 'I', 'N', ' ', 
'C', 'E', 'R', 'T', 'I', 'F', 'I', 'C', 'A', 'T', 'E', 
'-', '-', '-', '-', '-', '\n', 'M', 'I', 'I', 'B', 't', 
'j', 'C', 'C', 'A', 'V', 'u', 'g', 'A', 'w', 'I', 'B', 
'A', 'g', 'I', 'T', 'B', 'm', 'y', 'f', '1', 'X', 'S', 
'X', 'N', 'm', 'Y', '/', 'O', 'w', 'u', 'a', '2', 'e', 
'i', 'e', 'd', 'g', 'P', 'y', 'S', 'j', 'A', 'K', 'B', 
'g', 'g', 'q', 'h', 'k', 'j', 'O', 'P', 'Q', 'Q', 'D', 
'A', 'j', 'A', '5', '\n', 'M', 'Q', 's', 'w', 'C', 'Q', 
'Y', 'D', 'V', 'Q', 'Q', 'G', 'E', 'w', 'J', 'V', 'U', 
'z', 'E', 'P', 'M', 'A', '0', 'G', 'A', '1', 'U', 'E', 
'C', 'h', 'M', 'G', 'Q', 'W', '1', 'h', 'e', 'm', '9', 
'u', 'M', 'R', 'k', 'w', 'F', 'w', 'Y', 'D', 'V', 'Q', 
'Q', 'D', 'E', 'x', 'B', 'B', 'b', 'W', 'F', '6', 'b', 
'2', '4', 'g', '\n', 'U', 'm', '9', 'v', 'd', 'C', 'B', 
'D', 'Q', 'S', 'A', 'z', 'M', 'B', '4', 'X', 'D', 'T', 
'E', '1', 'M', 'D', 'U', 'y', 'N', 'j', 'A', 'w', 'M', 
'D', 'A', 'w', 'M', 'F', 'o', 'X', 'D', 'T', 'Q', 'w', 
'M', 'D', 'U', 'y', 'N', 'j', 'A', 'w', 'M', 'D', 'A', 
'w', 'M', 'F', 'o', 'w', 'O', 'T', 'E', 'L', 'M', 'A', 
'k', 'G', '\n', 'A', '1', 'U', 'E', 'B', 'h', 'M', 'C', 
'V', 'V', 'M', 'x', 'D', 'z', 'A', 'N', 'B', 'g', 'N', 
'V', 'B', 'A', 'o', 'T', 'B', 'k', 'F', 't', 'Y', 'X', 
'p', 'v', 'b', 'j', 'E', 'Z', 'M', 'B', 'c', 'G', 'A', 
'1', 'U', 'E', 'A', 'x', 'M', 'Q', 'Q', 'W', '1', 'h', 
'e', 'm', '9', 'u', 'I', 'F', 'J', 'v', 'b', '3', 'Q', 
'g', '\n', 'Q', '0', 'E', 'g', 'M', 'z', 'B', 'Z', 'M', 
'B', 'M', 'G', 'B', 'y', 'q', 'G', 'S', 'M', '4', '9', 
'A', 'g', 'E', 'G', 'C', 'C', 'q', 'G', 'S', 'M', '4', 
'9', 'A', 'w', 'E', 'H', 'A', '0', 'I', 'A', 'B', 'C', 
'm', 'X', 'p', '8', 'Z', 'B', 'f', '8', 'A', 'N', 'm', 
'+', 'g', 'B', 'G', '1', 'b', 'G', '8', 'l', 'K', 'l', 
'\n', 'u', 'i', '2', 'y', 'E', 'u', 'j', 'S', 'L', 't', 
'f', '6', 'y', 'c', 'X', 'Y', 'q', 'm', '0', 'f', 'c', 
'4', 'E', '7', 'O', '5', 'h', 'r', 'O', 'X', 'w', 'z', 
'p', 'c', 'V', 'O', 'h', 'o', '6', 'A', 'F', '2', 'h', 
'i', 'R', 'V', 'd', '9', 'R', 'F', 'g', 'd', 's', 'z', 
'f', 'l', 'Z', 'w', 'j', 'r', 'Z', 't', '6', 'j', '\n', 
'Q', 'j', 'B', 'A', 'M', 'A', '8', 'G', 'A', '1', 'U', 
'd', 'E', 'w', 'E', 'B', '/', 'w', 'Q', 'F', 'M', 'A', 
'M', 'B', 'A', 'f', '8', 'w', 'D', 'g', 'Y', 'D', 'V', 
'R', '0', 'P', 'A', 'Q', 'H', '/', 'B', 'A', 'Q', 'D', 
'A', 'g', 'G', 'G', 'M', 'B', '0', 'G', 'A', '1', 'U', 
'd', 'D', 'g', 'Q', 'W', 'B', 'B', 'S', 'r', '\n', 't', 
't', 'v', 'X', 'B', 'p', '4', '3', 'r', 'D', 'C', 'G', 
'B', '5', 'F', 'w', 'x', '5', 'z', 'E', 'G', 'b', 'F', 
'4', 'w', 'D', 'A', 'K', 'B', 'g', 'g', 'q', 'h', 'k', 
'j', 'O', 'P', 'Q', 'Q', 'D', 'A', 'g', 'N', 'J', 'A', 
'D', 'B', 'G', 'A', 'i', 'E', 'A', '4', 'I', 'W', 'S', 
'o', 'x', 'e', '3', 'j', 'f', 'k', 'r', '\n', 'B', 'q', 
'W', 'T', 'r', 'B', 'q', 'Y', 'a', 'G', 'F', 'y', '+', 
'u', 'G', 'h', '0', 'P', 's', 'c', 'e', 'G', 'C', 'm', 
'Q', '5', 'n', 'F', 'u', 'M', 'Q', 'C', 'I', 'Q', 'C', 
'c', 'A', 'u', '/', 'x', 'l', 'J', 'y', 'z', 'l', 'v', 
'n', 'r', 'x', 'i', 'r', '4', 't', 'i', 'z', '+', 'O', 
'p', 'A', 'U', 'F', 't', 'e', 'M', '\n', 'Y', 'y', 'R', 
'I', 'H', 'N', '8', 'w', 'f', 'd', 'V', 'o', 'O', 'w', 
'=', '=', '\n', '-', '-', '-', '-', '-', 'E', 'N', 'D', 
' ', 'C', 'E', 'R', 'T', 'I', 'F', 'I', 'C', 'A', 'T', 
'E', '-', '-', '-', '-', '-', '\0' };

extern char const CERTIFICATE[1 + 1];

extern char const PRIVATE_KEY[1 + 1];

bool AWSIoTX509CertStoreImpl_x509DataProvider_storeX509DataItem(X509DataItemKind_t kind, uint8_t const  *pDataItem, size_t dataItemSize, void *___id)
{
  return true;
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadRootCA(uint8_t index, uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  if (index == 0) 
  {
    *ppDataItem = ((uint8_t const  *)(AWS_IOT_ROOT_CA_1));
    *pDataItemSize = strlen(AWS_IOT_ROOT_CA_1) + 1;
  }
  else if (index == 1) {
    *ppDataItem = ((uint8_t const  *)(AWS_IOT_ROOT_CA_3));
    *pDataItemSize = strlen(AWS_IOT_ROOT_CA_3) + 1;
  }
  return true;
}

void AWSIoTX509CertStoreImpl_x509DataProvider_unloadX509DataItem(uint8_t const  **ppDataItem, void *___id)
{
  
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadCertificate(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  *ppDataItem = ((uint8_t const  *)(CERTIFICATE));
  *pDataItemSize = strlen(CERTIFICATE) + 1;
  return true;
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadPrivateKey(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  *ppDataItem = ((uint8_t const  *)(PRIVATE_KEY));
  *pDataItemSize = strlen(PRIVATE_KEY) + 1;
  return true;
}

size_t AWSIoTX509CertStoreImpl_x509DataProvider_getX509RootCACount(void *___id)
{
  return 2;
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadClientId(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  return false;
}
