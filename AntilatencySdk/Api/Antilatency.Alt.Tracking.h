//Copyright 2021, ALT LLC. All Rights Reserved.
//This file is part of Antilatency SDK.
//It is subject to the license terms in the LICENSE file found in the top-level directory
//of this distribution and at http://www.antilatency.com/eula
//You may not use this file except in compliance with the License.
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
#pragma once
#ifndef ANTILATENCY_INTERFACE_CONTRACT_EXCEPTIONS_ENABLED
#define ANTILATENCY_INTERFACE_CONTRACT_EXCEPTIONS_ENABLED
#endif
#include <Antilatency.InterfaceContract.h>
#include <Antilatency.Math.h>
#include <Antilatency.DeviceNetwork.h>
#include <cstdint>
#include <Antilatency.Alt.Environment.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4946)
#pragma warning(disable: 4265)
#endif
namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			enum class Stage : int32_t {
				InertialDataInitialization = 0x0,
				Tracking3Dof = 0x1,
				Tracking6Dof = 0x2,
				TrackingBlind6Dof = 0x3
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	inline const char* enumToString(const Antilatency::Alt::Tracking::Stage& x) {
		switch (x) {
			case Antilatency::Alt::Tracking::Stage::InertialDataInitialization: return "InertialDataInitialization";
			case Antilatency::Alt::Tracking::Stage::Tracking3Dof: return "Tracking3Dof";
			case Antilatency::Alt::Tracking::Stage::Tracking6Dof: return "Tracking6Dof";
			case Antilatency::Alt::Tracking::Stage::TrackingBlind6Dof: return "TrackingBlind6Dof";
		}
		return "";
	}
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			struct Stability {
				Antilatency::Alt::Tracking::Stage stage;
				float value;
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			struct State {
				Antilatency::Math::floatP3Q pose;
				Antilatency::Math::float3 velocity;
				Antilatency::Math::float3 localAngularVelocity;
				Antilatency::Alt::Tracking::Stability stability;
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			/* copy and paste this to implementer
			Antilatency::Alt::Tracking::State getExtrapolatedState(Antilatency::Math::floatP3Q placement, float deltaTime) {
				throw std::logic_error{"Method ITrackingCotask.getExtrapolatedState() is not implemented."};
			}
			Antilatency::Alt::Tracking::State getState(float angularVelocityAvgTimeInSeconds) {
				throw std::logic_error{"Method ITrackingCotask.getState() is not implemented."};
			}
			*/
			struct ITrackingCotask : Antilatency::DeviceNetwork::ICotask {
				struct VMT : Antilatency::DeviceNetwork::ICotask::VMT {
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getExtrapolatedState(Antilatency::Math::floatP3Q placement, float deltaTime, Antilatency::Alt::Tracking::State& result) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getState(float angularVelocityAvgTimeInSeconds, Antilatency::Alt::Tracking::State& result) = 0;
					static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
						return Antilatency::InterfaceContract::InterfaceID{0x7f8b603c,0xfa91,0x4168,{0x92,0xb7,0xaf,0x16,0x44,0xd0,0x87,0xdb}};
					}
				private:
					~VMT() = delete;
				};
				ITrackingCotask() = default;
				ITrackingCotask(std::nullptr_t) {}
				explicit ITrackingCotask(VMT* pointer) : Antilatency::DeviceNetwork::ICotask(pointer) {}
				template<typename T, typename = typename std::enable_if<std::is_base_of<ITrackingCotask, T>::value>::type>
				ITrackingCotask& operator = (const T& other) {
				    Antilatency::DeviceNetwork::ICotask::operator=(other);
				    return *this;
				}
				template<class Implementer, class ... TArgs>
				static ITrackingCotask create(TArgs&&... args) {
				    return *new Implementer(std::forward<TArgs>(args)...);
				}
				void attach(VMT* other) ANTILATENCY_NOEXCEPT {
				    Antilatency::DeviceNetwork::ICotask::attach(other);
				}
				VMT* detach() ANTILATENCY_NOEXCEPT {
				    return reinterpret_cast<VMT*>(Antilatency::DeviceNetwork::ICotask::detach());
				}
				Antilatency::Alt::Tracking::State getExtrapolatedState(Antilatency::Math::floatP3Q placement, float deltaTime) {
					Antilatency::Alt::Tracking::State result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getExtrapolatedState(placement, deltaTime, result));
					return result;
				}
				Antilatency::Alt::Tracking::State getState(float angularVelocityAvgTimeInSeconds) {
					Antilatency::Alt::Tracking::State result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getState(angularVelocityAvgTimeInSeconds, result));
					return result;
				}
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Tracking::ITrackingCotask, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Tracking::ITrackingCotask::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getExtrapolatedState(Antilatency::Math::floatP3Q placement, float deltaTime, Antilatency::Alt::Tracking::State& result) {
					try {
						result = this->_object->getExtrapolatedState(placement, deltaTime);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getState(float angularVelocityAvgTimeInSeconds, Antilatency::Alt::Tracking::State& result) {
					try {
						result = this->_object->getState(angularVelocityAvgTimeInSeconds);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
			};
		} //namespace Details
	} //namespace InterfaceContract
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			/* copy and paste this to implementer
			Antilatency::Alt::Tracking::ITrackingCotask startTask(Antilatency::DeviceNetwork::INetwork network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Environment::IEnvironment environment) {
				throw std::logic_error{"Method ITrackingCotaskConstructor.startTask() is not implemented."};
			}
			*/
			struct ITrackingCotaskConstructor : Antilatency::DeviceNetwork::ICotaskConstructor {
				struct VMT : Antilatency::DeviceNetwork::ICotaskConstructor::VMT {
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL startTask(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Environment::IEnvironment::VMT* environment, Antilatency::Alt::Tracking::ITrackingCotask& result) = 0;
					static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
						return Antilatency::InterfaceContract::InterfaceID{0x009ebfe1,0xf85c,0x4638,{0xbe,0x9d,0xaf,0x79,0x90,0xa8,0xcd,0x04}};
					}
				private:
					~VMT() = delete;
				};
				ITrackingCotaskConstructor() = default;
				ITrackingCotaskConstructor(std::nullptr_t) {}
				explicit ITrackingCotaskConstructor(VMT* pointer) : Antilatency::DeviceNetwork::ICotaskConstructor(pointer) {}
				template<typename T, typename = typename std::enable_if<std::is_base_of<ITrackingCotaskConstructor, T>::value>::type>
				ITrackingCotaskConstructor& operator = (const T& other) {
				    Antilatency::DeviceNetwork::ICotaskConstructor::operator=(other);
				    return *this;
				}
				template<class Implementer, class ... TArgs>
				static ITrackingCotaskConstructor create(TArgs&&... args) {
				    return *new Implementer(std::forward<TArgs>(args)...);
				}
				void attach(VMT* other) ANTILATENCY_NOEXCEPT {
				    Antilatency::DeviceNetwork::ICotaskConstructor::attach(other);
				}
				VMT* detach() ANTILATENCY_NOEXCEPT {
				    return reinterpret_cast<VMT*>(Antilatency::DeviceNetwork::ICotaskConstructor::detach());
				}
				Antilatency::Alt::Tracking::ITrackingCotask startTask(Antilatency::DeviceNetwork::INetwork network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Environment::IEnvironment environment) {
					Antilatency::Alt::Tracking::ITrackingCotask result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->startTask(network.detach(), node, environment.detach(), result));
					return result;
				}
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Tracking::ITrackingCotaskConstructor, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotaskConstructor, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Tracking::ITrackingCotaskConstructor::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotaskConstructor, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL startTask(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Environment::IEnvironment::VMT* environment, Antilatency::Alt::Tracking::ITrackingCotask& result) {
					try {
						Antilatency::DeviceNetwork::INetwork networkMarshaler;
						networkMarshaler.attach(network);
						Antilatency::Alt::Environment::IEnvironment environmentMarshaler;
						environmentMarshaler.attach(environment);
						result = this->_object->startTask(networkMarshaler, node, environmentMarshaler);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
			};
		} //namespace Details
	} //namespace InterfaceContract
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			/* copy and paste this to implementer
			Antilatency::Math::floatQ getOpticsToBodyRotation() {
				throw std::logic_error{"Method ITrackingDataCotask.getOpticsToBodyRotation() is not implemented."};
			}
			void setExposure(uint32_t exposure) {
				throw std::logic_error{"Method ITrackingDataCotask.setExposure() is not implemented."};
			}
			*/
			struct ITrackingDataCotask : Antilatency::DeviceNetwork::ICotask {
				struct VMT : Antilatency::DeviceNetwork::ICotask::VMT {
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getOpticsToBodyRotation(Antilatency::Math::floatQ& result) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setExposure(uint32_t exposure) = 0;
					static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
						return Antilatency::InterfaceContract::InterfaceID{0x91b0a5be,0xa9c7,0x4d29,{0xa0,0x3a,0x44,0xff,0xf8,0xe9,0x1c,0x68}};
					}
				private:
					~VMT() = delete;
				};
				ITrackingDataCotask() = default;
				ITrackingDataCotask(std::nullptr_t) {}
				explicit ITrackingDataCotask(VMT* pointer) : Antilatency::DeviceNetwork::ICotask(pointer) {}
				template<typename T, typename = typename std::enable_if<std::is_base_of<ITrackingDataCotask, T>::value>::type>
				ITrackingDataCotask& operator = (const T& other) {
				    Antilatency::DeviceNetwork::ICotask::operator=(other);
				    return *this;
				}
				template<class Implementer, class ... TArgs>
				static ITrackingDataCotask create(TArgs&&... args) {
				    return *new Implementer(std::forward<TArgs>(args)...);
				}
				void attach(VMT* other) ANTILATENCY_NOEXCEPT {
				    Antilatency::DeviceNetwork::ICotask::attach(other);
				}
				VMT* detach() ANTILATENCY_NOEXCEPT {
				    return reinterpret_cast<VMT*>(Antilatency::DeviceNetwork::ICotask::detach());
				}
				Antilatency::Math::floatQ getOpticsToBodyRotation() {
					Antilatency::Math::floatQ result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getOpticsToBodyRotation(result));
					return result;
				}
				void setExposure(uint32_t exposure) {
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->setExposure(exposure));
				}
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Tracking::ITrackingDataCotask, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Tracking::ITrackingDataCotask::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getOpticsToBodyRotation(Antilatency::Math::floatQ& result) {
					try {
						result = this->_object->getOpticsToBodyRotation();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setExposure(uint32_t exposure) {
					try {
						this->_object->setExposure(exposure);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
			};
		} //namespace Details
	} //namespace InterfaceContract
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			struct OpticRay {
				Antilatency::Math::float2 middleSpacePoint;
				Antilatency::Math::float2x3 middleSpacePointDerivativeByLocalPosition;
				Antilatency::Math::float3 direction;
				float power;
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			struct InertialLeap {
				float timeLeap;
				Antilatency::Math::float3 positionLeap;
				Antilatency::Math::float3 velocityLeap;
				Antilatency::Math::floatQ rotationLeap;
				Antilatency::Math::float3x3 positionLeapByAcceleration;
				Antilatency::Math::float3x3 velocityLeapByAcceleration;
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			/* copy and paste this to implementer
			void onTrackingFrame(const std::vector<Antilatency::Alt::Tracking::OpticRay>& rays, Antilatency::Math::float3 inertialUp, Antilatency::Alt::Tracking::InertialLeap inertialLeap, Antilatency::InterfaceContract::LongBool accelerometerOverflowOccured, Antilatency::InterfaceContract::LongBool gyroscopeOverflowOccured) {
				throw std::logic_error{"Method ITrackingDataCallback.onTrackingFrame() is not implemented."};
			}
			void onIncompleteLeap(Antilatency::Alt::Tracking::InertialLeap leap) {
				throw std::logic_error{"Method ITrackingDataCallback.onIncompleteLeap() is not implemented."};
			}
			void onAdnFinalize() {
				throw std::logic_error{"Method ITrackingDataCallback.onAdnFinalize() is not implemented."};
			}
			*/
			struct ITrackingDataCallback : Antilatency::InterfaceContract::IInterface {
				struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL onTrackingFrame(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate rays, Antilatency::Math::float3 inertialUp, Antilatency::Alt::Tracking::InertialLeap inertialLeap, Antilatency::InterfaceContract::LongBool accelerometerOverflowOccured, Antilatency::InterfaceContract::LongBool gyroscopeOverflowOccured) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL onIncompleteLeap(Antilatency::Alt::Tracking::InertialLeap leap) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL onAdnFinalize() = 0;
					static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
						return Antilatency::InterfaceContract::InterfaceID{0x8f2dab91,0x8ba5,0x40a3,{0xae,0x73,0xe3,0xf8,0xef,0x1f,0xb8,0x76}};
					}
				private:
					~VMT() = delete;
				};
				ITrackingDataCallback() = default;
				ITrackingDataCallback(std::nullptr_t) {}
				explicit ITrackingDataCallback(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
				template<typename T, typename = typename std::enable_if<std::is_base_of<ITrackingDataCallback, T>::value>::type>
				ITrackingDataCallback& operator = (const T& other) {
				    Antilatency::InterfaceContract::IInterface::operator=(other);
				    return *this;
				}
				template<class Implementer, class ... TArgs>
				static ITrackingDataCallback create(TArgs&&... args) {
				    return *new Implementer(std::forward<TArgs>(args)...);
				}
				void attach(VMT* other) ANTILATENCY_NOEXCEPT {
				    Antilatency::InterfaceContract::IInterface::attach(other);
				}
				VMT* detach() ANTILATENCY_NOEXCEPT {
				    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
				}
				void onTrackingFrame(const std::vector<Antilatency::Alt::Tracking::OpticRay>& rays, Antilatency::Math::float3 inertialUp, Antilatency::Alt::Tracking::InertialLeap inertialLeap, Antilatency::InterfaceContract::LongBool accelerometerOverflowOccured, Antilatency::InterfaceContract::LongBool gyroscopeOverflowOccured) {
					auto raysMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(rays);
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->onTrackingFrame(raysMarshaler, inertialUp, inertialLeap, accelerometerOverflowOccured, gyroscopeOverflowOccured));
				}
				void onIncompleteLeap(Antilatency::Alt::Tracking::InertialLeap leap) {
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->onIncompleteLeap(leap));
				}
				void onAdnFinalize() {
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->onAdnFinalize());
				}
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Tracking::ITrackingDataCallback, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Tracking::ITrackingDataCallback::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL onTrackingFrame(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate rays, Antilatency::Math::float3 inertialUp, Antilatency::Alt::Tracking::InertialLeap inertialLeap, Antilatency::InterfaceContract::LongBool accelerometerOverflowOccured, Antilatency::InterfaceContract::LongBool gyroscopeOverflowOccured) {
					try {
						this->_object->onTrackingFrame(rays, inertialUp, inertialLeap, accelerometerOverflowOccured, gyroscopeOverflowOccured);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL onIncompleteLeap(Antilatency::Alt::Tracking::InertialLeap leap) {
					try {
						this->_object->onIncompleteLeap(leap);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL onAdnFinalize() {
					try {
						this->_object->onAdnFinalize();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
			};
		} //namespace Details
	} //namespace InterfaceContract
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			/* copy and paste this to implementer
			Antilatency::Alt::Tracking::ITrackingDataCotask startTask(Antilatency::DeviceNetwork::INetwork network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Tracking::ITrackingDataCallback callback) {
				throw std::logic_error{"Method ITrackingDataCotaskConstructor.startTask() is not implemented."};
			}
			*/
			struct ITrackingDataCotaskConstructor : Antilatency::DeviceNetwork::ICotaskConstructor {
				struct VMT : Antilatency::DeviceNetwork::ICotaskConstructor::VMT {
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL startTask(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Tracking::ITrackingDataCallback::VMT* callback, Antilatency::Alt::Tracking::ITrackingDataCotask& result) = 0;
					static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
						return Antilatency::InterfaceContract::InterfaceID{0xb3032673,0x093a,0x47c5,{0xa0,0x49,0x31,0x57,0x6d,0xcb,0xe8,0x94}};
					}
				private:
					~VMT() = delete;
				};
				ITrackingDataCotaskConstructor() = default;
				ITrackingDataCotaskConstructor(std::nullptr_t) {}
				explicit ITrackingDataCotaskConstructor(VMT* pointer) : Antilatency::DeviceNetwork::ICotaskConstructor(pointer) {}
				template<typename T, typename = typename std::enable_if<std::is_base_of<ITrackingDataCotaskConstructor, T>::value>::type>
				ITrackingDataCotaskConstructor& operator = (const T& other) {
				    Antilatency::DeviceNetwork::ICotaskConstructor::operator=(other);
				    return *this;
				}
				template<class Implementer, class ... TArgs>
				static ITrackingDataCotaskConstructor create(TArgs&&... args) {
				    return *new Implementer(std::forward<TArgs>(args)...);
				}
				void attach(VMT* other) ANTILATENCY_NOEXCEPT {
				    Antilatency::DeviceNetwork::ICotaskConstructor::attach(other);
				}
				VMT* detach() ANTILATENCY_NOEXCEPT {
				    return reinterpret_cast<VMT*>(Antilatency::DeviceNetwork::ICotaskConstructor::detach());
				}
				Antilatency::Alt::Tracking::ITrackingDataCotask startTask(Antilatency::DeviceNetwork::INetwork network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Tracking::ITrackingDataCallback callback) {
					Antilatency::Alt::Tracking::ITrackingDataCotask result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->startTask(network.detach(), node, callback.detach(), result));
					return result;
				}
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotaskConstructor, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotaskConstructor, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL startTask(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::Alt::Tracking::ITrackingDataCallback::VMT* callback, Antilatency::Alt::Tracking::ITrackingDataCotask& result) {
					try {
						Antilatency::DeviceNetwork::INetwork networkMarshaler;
						networkMarshaler.attach(network);
						Antilatency::Alt::Tracking::ITrackingDataCallback callbackMarshaler;
						callbackMarshaler.attach(callback);
						result = this->_object->startTask(networkMarshaler, node, callbackMarshaler);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
			};
		} //namespace Details
	} //namespace InterfaceContract
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			/* copy and paste this to implementer
			Antilatency::Math::floatP3Q createPlacement(const std::string& code) {
				throw std::logic_error{"Method ILibrary.createPlacement() is not implemented."};
			}
			std::string encodePlacement(Antilatency::Math::float3 position, Antilatency::Math::float3 rotation) {
				throw std::logic_error{"Method ILibrary.encodePlacement() is not implemented."};
			}
			Antilatency::Alt::Tracking::ITrackingCotaskConstructor createTrackingCotaskConstructor() {
				throw std::logic_error{"Method ILibrary.createTrackingCotaskConstructor() is not implemented."};
			}
			Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor createTrackingDataCotaskConstructor() {
				throw std::logic_error{"Method ILibrary.createTrackingDataCotaskConstructor() is not implemented."};
			}
			*/
			struct ILibrary : Antilatency::InterfaceContract::IInterface {
				struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createPlacement(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate code, Antilatency::Math::floatP3Q& result) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL encodePlacement(Antilatency::Math::float3 position, Antilatency::Math::float3 rotation, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createTrackingCotaskConstructor(Antilatency::Alt::Tracking::ITrackingCotaskConstructor& result) = 0;
					virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createTrackingDataCotaskConstructor(Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor& result) = 0;
					static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
						return Antilatency::InterfaceContract::InterfaceID{0xad4034ee,0xb77a,0x4e1f,{0xa5,0x5a,0xc4,0xb3,0x0a,0x46,0x9a,0x24}};
					}
				private:
					~VMT() = delete;
				};
				ILibrary() = default;
				ILibrary(std::nullptr_t) {}
				explicit ILibrary(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
				template<typename T, typename = typename std::enable_if<std::is_base_of<ILibrary, T>::value>::type>
				ILibrary& operator = (const T& other) {
				    Antilatency::InterfaceContract::IInterface::operator=(other);
				    return *this;
				}
				template<class Implementer, class ... TArgs>
				static ILibrary create(TArgs&&... args) {
				    return *new Implementer(std::forward<TArgs>(args)...);
				}
				void attach(VMT* other) ANTILATENCY_NOEXCEPT {
				    Antilatency::InterfaceContract::IInterface::attach(other);
				}
				VMT* detach() ANTILATENCY_NOEXCEPT {
				    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
				}
				Antilatency::Math::floatP3Q createPlacement(const std::string& code) {
					Antilatency::Math::floatP3Q result;
					auto codeMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(code);
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->createPlacement(codeMarshaler, result));
					return result;
				}
				std::string encodePlacement(Antilatency::Math::float3 position, Antilatency::Math::float3 rotation) {
					std::string result;
					auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->encodePlacement(position, rotation, resultMarshaler));
					return result;
				}
				Antilatency::Alt::Tracking::ITrackingCotaskConstructor createTrackingCotaskConstructor() {
					Antilatency::Alt::Tracking::ITrackingCotaskConstructor result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->createTrackingCotaskConstructor(result));
					return result;
				}
				Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor createTrackingDataCotaskConstructor() {
					Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor result;
					handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->createTrackingDataCotaskConstructor(result));
					return result;
				}
			};
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Tracking::ILibrary, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Tracking::ILibrary::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createPlacement(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate code, Antilatency::Math::floatP3Q& result) {
					try {
						result = this->_object->createPlacement(code);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL encodePlacement(Antilatency::Math::float3 position, Antilatency::Math::float3 rotation, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->encodePlacement(position, rotation);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createTrackingCotaskConstructor(Antilatency::Alt::Tracking::ITrackingCotaskConstructor& result) {
					try {
						result = this->_object->createTrackingCotaskConstructor();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createTrackingDataCotaskConstructor(Antilatency::Alt::Tracking::ITrackingDataCotaskConstructor& result) {
					try {
						result = this->_object->createTrackingDataCotaskConstructor();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
			};
		} //namespace Details
	} //namespace InterfaceContract
} //namespace Antilatency

namespace Antilatency {
	namespace Alt {
		namespace Tracking {
			namespace Constants {
				const Antilatency::Math::float3 InitialPositionFor3Dof = ([](){
					Antilatency::Math::float3 result;
					(*reinterpret_cast<std::array<uint8_t, 12>*>(&result)) = {{0U, 0U, 0U, 0U, 61U, 10U, 215U, 63U, 0U, 0U, 0U, 0U}};
					return result;
				})();
				constexpr float DefaultAngularVelocityAvgTime = 0.016f;
			} //namespace Constants
		} //namespace Tracking
	} //namespace Alt
} //namespace Antilatency


#ifdef _MSC_VER
#pragma warning(pop)
#endif
