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
#include "DemoFOTAUpdateWorkflowFilePersistence.h"

#include "DemoFOTAUpdateWorkflowPersistence.h"

void saveState(int16_t state)
{
  FILE *file = fopen(DEMO_PRODUCT_UPDATE_STATE_FILE_NAME, "wb");
  if (file == NULL) 
  {
    printf("Failed to open %s for writing\n", DEMO_PRODUCT_UPDATE_STATE_FILE_NAME);
    return;
  }
  if (fwrite(&state, sizeof(state), 1, file) != 1) 
  {
    printf("Failed to write statemachine state %hi to %s\n", state, DEMO_PRODUCT_UPDATE_STATE_FILE_NAME);
    fclose(file);
    return;
  }
  fclose(file);
}

int16_t loadState(void)
{
  FILE *file = fopen(DEMO_PRODUCT_UPDATE_STATE_FILE_NAME, "rb");
  if (file == NULL) 
  {
    return 0;
  }
  int16_t state = 0;
  if (fread(&state, sizeof(state), 1, file) != 1) 
  {
    fclose(file);
    return 0;
  }
  fclose(file);
  return state;
}
