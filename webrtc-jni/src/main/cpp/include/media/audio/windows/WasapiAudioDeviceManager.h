/*
 * Copyright 2019 Alex Andres
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
 */

#ifndef JNI_WEBRTC_MEDIA_WASAPI_AUDIO_DEVICE_MANAGER_H_
#define JNI_WEBRTC_MEDIA_WASAPI_AUDIO_DEVICE_MANAGER_H_

#include "media/audio/AudioDeviceManager.h"
#include "platform/windows/ComInitializer.h"
#include "platform/windows/ComPtr.h"

#include <mmdeviceapi.h>

namespace jni
{
	namespace avdev
	{
		class WasapiAudioDeviceManager : public AudioDeviceManager, IMMNotificationClient
		{
			public:
				WasapiAudioDeviceManager();
				~WasapiAudioDeviceManager();

				std::set<AudioDevicePtr> getAudioCaptureDevices() override;
				std::set<AudioDevicePtr> getAudioPlaybackDevices() override;

			private:
				void enumerateDevices(EDataFlow dataFlow);
				void addDevice(LPCWSTR deviceId);
				void removeDevice(LPCWSTR deviceId);
				std::shared_ptr<AudioDevice> createAudioDevice(LPCWSTR deviceId, EDataFlow * dataFlow);

				template <typename T>
				void removeAudioDevice(DeviceList<T> & devices, std::string id);

				// IMMNotificationClient implementation.
				STDMETHOD_(ULONG, AddRef)();
				STDMETHOD_(ULONG, Release)();
				STDMETHOD(QueryInterface)(REFIID iid, void ** object);
				STDMETHOD(OnDefaultDeviceChanged) (EDataFlow flow, ERole role, LPCWSTR deviceId);
				STDMETHOD(OnDeviceAdded) (LPCWSTR deviceId);
				STDMETHOD(OnDeviceRemoved) (LPCWSTR deviceId);
				STDMETHOD(OnDeviceStateChanged) (LPCWSTR deviceId, DWORD newState);
				STDMETHOD(OnPropertyValueChanged) (LPCWSTR /*deviceId*/, const PROPERTYKEY /*key*/);

				ComInitializer initializer;
				ComPtr<IMMDeviceEnumerator> deviceEnumerator;
		};
	}
}

#endif