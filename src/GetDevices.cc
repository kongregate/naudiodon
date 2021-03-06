/* Copyright 2017 Streampunk Media Ltd.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "GetDevices.h"

int paInitialized = false;

PaError EnsureInitialized() {
  PaError err;

  if(!paInitialized) {
    err = Pa_Initialize();
    if(err != paNoError) {
      return err;
    };
    paInitialized = true;
  }
  return paNoError;
}

NAN_METHOD(GetDevices) {
  char str[1000];
  int numDevices;

  PaError err = EnsureInitialized();
  if(err != paNoError) {
    sprintf(str, "Could not initialize PortAudio %d", err);
    return Nan::ThrowError(str);
  }

  numDevices = Pa_GetDeviceCount();
  v8::Local<v8::Array> result = Nan::New<v8::Array>(numDevices);

  for ( int i = 0 ; i < numDevices ; i++) {
    const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
    v8::Local<v8::Object> v8DeviceInfo = Nan::New<v8::Object>();
    Nan::Set(v8DeviceInfo, Nan::New("id").ToLocalChecked(), Nan::New(i));
    Nan::Set(v8DeviceInfo, Nan::New("name").ToLocalChecked(),
      Nan::New(deviceInfo->name).ToLocalChecked());
    Nan::Set(v8DeviceInfo, Nan::New("maxInputChannels").ToLocalChecked(),
      Nan::New(deviceInfo->maxInputChannels));
    Nan::Set(v8DeviceInfo, Nan::New("maxOutputChannels").ToLocalChecked(),
      Nan::New(deviceInfo->maxOutputChannels));
    Nan::Set(v8DeviceInfo, Nan::New("defaultSampleRate").ToLocalChecked(),
      Nan::New(deviceInfo->defaultSampleRate));
    Nan::Set(v8DeviceInfo, Nan::New("defaultLowInputLatency").ToLocalChecked(),
      Nan::New(deviceInfo->defaultLowInputLatency));
    Nan::Set(v8DeviceInfo, Nan::New("defaultLowOutputLatency").ToLocalChecked(),
      Nan::New(deviceInfo->defaultLowOutputLatency));
    Nan::Set(v8DeviceInfo, Nan::New("defaultHighInputLatency").ToLocalChecked(),
      Nan::New(deviceInfo->defaultHighInputLatency));
    Nan::Set(v8DeviceInfo, Nan::New("defaultHighOutputLatency").ToLocalChecked(),
      Nan::New(deviceInfo->defaultHighOutputLatency));
    Nan::Set(v8DeviceInfo, Nan::New("hostAPIName").ToLocalChecked(),
      Nan::New(Pa_GetHostApiInfo(deviceInfo->hostApi)->name).ToLocalChecked());

    Nan::Set(result, i, v8DeviceInfo);
  }

  info.GetReturnValue().Set(result);
}
