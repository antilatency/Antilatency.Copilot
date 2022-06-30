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
#include <cstdint>
#include <Antilatency.DeviceNetwork.Interop.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4946)
#pragma warning(disable: 4265)
#endif
namespace Antilatency {
	namespace DeviceNetwork {
		enum class UsbVendorId : uint16_t {
			Antilatency = 0x3237
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	inline const char* enumToString(const Antilatency::DeviceNetwork::UsbVendorId& x) {
		switch (x) {
			case Antilatency::DeviceNetwork::UsbVendorId::Antilatency: return "Antilatency";
		}
		return "";
	}
} //namespace Antilatency

namespace Antilatency {
	namespace DeviceNetwork {
		struct UsbDeviceFilter {
			Antilatency::DeviceNetwork::UsbVendorId vid;
			uint16_t pid;
			uint16_t pidMask;
		};
	} //namespace DeviceNetwork
} //namespace Antilatency

namespace Antilatency {
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		void addUsbDevice(Antilatency::DeviceNetwork::UsbDeviceFilter usbDeviceType) {
			throw std::logic_error{"Method IDeviceFilter.addUsbDevice() is not implemented."};
		}
		void addIpDevice(const std::string& ip, const std::string& mask) {
			throw std::logic_error{"Method IDeviceFilter.addIpDevice() is not implemented."};
		}
		Antilatency::DeviceNetwork::UsbDeviceFilter getUsbDevice(uint32_t index) {
			throw std::logic_error{"Method IDeviceFilter.getUsbDevice() is not implemented."};
		}
		std::string getIpDevice(uint32_t index) {
			throw std::logic_error{"Method IDeviceFilter.getIpDevice() is not implemented."};
		}
		std::string getIpDeviceMask(uint32_t index) {
			throw std::logic_error{"Method IDeviceFilter.getIpDeviceMask() is not implemented."};
		}
		*/
		struct IDeviceFilter : Antilatency::InterfaceContract::IInterface {
			struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL addUsbDevice(Antilatency::DeviceNetwork::UsbDeviceFilter usbDeviceType) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL addIpDevice(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate ip, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate mask) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getUsbDevice(uint32_t index, Antilatency::DeviceNetwork::UsbDeviceFilter& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getIpDevice(uint32_t index, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getIpDeviceMask(uint32_t index, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0x9e6bdb45,0xe7cb,0x4f46,{0xa5,0xee,0xce,0x73,0x53,0xa7,0x34,0xc9}};
				}
			private:
				~VMT() = delete;
			};
			IDeviceFilter() = default;
			IDeviceFilter(std::nullptr_t) {}
			explicit IDeviceFilter(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
			template<typename T, typename = typename std::enable_if<std::is_base_of<IDeviceFilter, T>::value>::type>
			IDeviceFilter& operator = (const T& other) {
			    Antilatency::InterfaceContract::IInterface::operator=(other);
			    return *this;
			}
			template<class Implementer, class ... TArgs>
			static IDeviceFilter create(TArgs&&... args) {
			    return *new Implementer(std::forward<TArgs>(args)...);
			}
			void attach(VMT* other) ANTILATENCY_NOEXCEPT {
			    Antilatency::InterfaceContract::IInterface::attach(other);
			}
			VMT* detach() ANTILATENCY_NOEXCEPT {
			    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
			}
			void addUsbDevice(Antilatency::DeviceNetwork::UsbDeviceFilter usbDeviceType) {
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->addUsbDevice(usbDeviceType));
			}
			void addIpDevice(const std::string& ip, const std::string& mask) {
				auto ipMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(ip);
				auto maskMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(mask);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->addIpDevice(ipMarshaler, maskMarshaler));
			}
			Antilatency::DeviceNetwork::UsbDeviceFilter getUsbDevice(uint32_t index) {
				Antilatency::DeviceNetwork::UsbDeviceFilter result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getUsbDevice(index, result));
				return result;
			}
			std::string getIpDevice(uint32_t index) {
				std::string result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getIpDevice(index, resultMarshaler));
				return result;
			}
			std::string getIpDeviceMask(uint32_t index) {
				std::string result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getIpDeviceMask(index, resultMarshaler));
				return result;
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::IDeviceFilter, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::IDeviceFilter::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL addUsbDevice(Antilatency::DeviceNetwork::UsbDeviceFilter usbDeviceType) {
					try {
						this->_object->addUsbDevice(usbDeviceType);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL addIpDevice(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate ip, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate mask) {
					try {
						this->_object->addIpDevice(ip, mask);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getUsbDevice(uint32_t index, Antilatency::DeviceNetwork::UsbDeviceFilter& result) {
					try {
						result = this->_object->getUsbDevice(index);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getIpDevice(uint32_t index, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getIpDevice(index);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getIpDeviceMask(uint32_t index, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getIpDeviceMask(index);
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
	namespace DeviceNetwork {
		enum class NodeHandle : uint32_t {
			Null = 0x0
		};
		ANTILATENCY_ENUM_INTEGER_BEHAVIOUR_UNSIGNED(NodeHandle,uint32_t)
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	inline const char* enumToString(const Antilatency::DeviceNetwork::NodeHandle& x) {
		switch (x) {
			case Antilatency::DeviceNetwork::NodeHandle::Null: return "Null";
		}
		return "";
	}
} //namespace Antilatency

namespace Antilatency {
	namespace DeviceNetwork {
		enum class NodeStatus : int32_t {
			Idle = 0x0,
			TaskRunning = 0x1,
			Invalid = 0x2
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	inline const char* enumToString(const Antilatency::DeviceNetwork::NodeStatus& x) {
		switch (x) {
			case Antilatency::DeviceNetwork::NodeStatus::Idle: return "Idle";
			case Antilatency::DeviceNetwork::NodeStatus::TaskRunning: return "TaskRunning";
			case Antilatency::DeviceNetwork::NodeStatus::Invalid: return "Invalid";
		}
		return "";
	}
} //namespace Antilatency

namespace Antilatency {
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		std::vector<Antilatency::DeviceNetwork::Interop::Packet> getPackets() {
			throw std::logic_error{"Method ISynchronousConnection.getPackets() is not implemented."};
		}
		std::vector<Antilatency::DeviceNetwork::Interop::Packet> getAvailablePackets(Antilatency::InterfaceContract::LongBool& taskFinished) {
			throw std::logic_error{"Method ISynchronousConnection.getAvailablePackets() is not implemented."};
		}
		Antilatency::InterfaceContract::LongBool writePacket(Antilatency::DeviceNetwork::Interop::Packet packet) {
			throw std::logic_error{"Method ISynchronousConnection.writePacket() is not implemented."};
		}
		*/
		struct ISynchronousConnection : Antilatency::InterfaceContract::IInterface {
			struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getPackets(Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getAvailablePackets(Antilatency::InterfaceContract::LongBool& taskFinished, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL writePacket(Antilatency::DeviceNetwork::Interop::Packet packet, Antilatency::InterfaceContract::LongBool& result) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0xaf7402e8,0x2a23,0x442b,{0x82,0x30,0xd4,0x1f,0x55,0xef,0x5d,0xc0}};
				}
			private:
				~VMT() = delete;
			};
			ISynchronousConnection() = default;
			ISynchronousConnection(std::nullptr_t) {}
			explicit ISynchronousConnection(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
			template<typename T, typename = typename std::enable_if<std::is_base_of<ISynchronousConnection, T>::value>::type>
			ISynchronousConnection& operator = (const T& other) {
			    Antilatency::InterfaceContract::IInterface::operator=(other);
			    return *this;
			}
			template<class Implementer, class ... TArgs>
			static ISynchronousConnection create(TArgs&&... args) {
			    return *new Implementer(std::forward<TArgs>(args)...);
			}
			void attach(VMT* other) ANTILATENCY_NOEXCEPT {
			    Antilatency::InterfaceContract::IInterface::attach(other);
			}
			VMT* detach() ANTILATENCY_NOEXCEPT {
			    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
			}
			std::vector<Antilatency::DeviceNetwork::Interop::Packet> getPackets() {
				std::vector<Antilatency::DeviceNetwork::Interop::Packet> result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getPackets(resultMarshaler));
				return result;
			}
			std::vector<Antilatency::DeviceNetwork::Interop::Packet> getAvailablePackets(Antilatency::InterfaceContract::LongBool& taskFinished) {
				std::vector<Antilatency::DeviceNetwork::Interop::Packet> result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getAvailablePackets(taskFinished, resultMarshaler));
				return result;
			}
			Antilatency::InterfaceContract::LongBool writePacket(Antilatency::DeviceNetwork::Interop::Packet packet) {
				Antilatency::InterfaceContract::LongBool result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->writePacket(packet, result));
				return result;
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::ISynchronousConnection, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::ISynchronousConnection::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getPackets(Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getPackets();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getAvailablePackets(Antilatency::InterfaceContract::LongBool& taskFinished, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getAvailablePackets(taskFinished);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL writePacket(Antilatency::DeviceNetwork::Interop::Packet packet, Antilatency::InterfaceContract::LongBool& result) {
					try {
						result = this->_object->writePacket(packet);
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
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		Antilatency::InterfaceContract::LongBool isTaskFinished() {
			throw std::logic_error{"Method ICotask.isTaskFinished() is not implemented."};
		}
		*/
		struct ICotask : Antilatency::InterfaceContract::IInterface {
			struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL isTaskFinished(Antilatency::InterfaceContract::LongBool& result) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0xfd95f649,0x562a,0x4819,{0xa8,0x16,0x26,0xb7,0x6c,0xd9,0xd8,0xd6}};
				}
			private:
				~VMT() = delete;
			};
			ICotask() = default;
			ICotask(std::nullptr_t) {}
			explicit ICotask(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
			template<typename T, typename = typename std::enable_if<std::is_base_of<ICotask, T>::value>::type>
			ICotask& operator = (const T& other) {
			    Antilatency::InterfaceContract::IInterface::operator=(other);
			    return *this;
			}
			template<class Implementer, class ... TArgs>
			static ICotask create(TArgs&&... args) {
			    return *new Implementer(std::forward<TArgs>(args)...);
			}
			void attach(VMT* other) ANTILATENCY_NOEXCEPT {
			    Antilatency::InterfaceContract::IInterface::attach(other);
			}
			VMT* detach() ANTILATENCY_NOEXCEPT {
			    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
			}
			Antilatency::InterfaceContract::LongBool isTaskFinished() {
				Antilatency::InterfaceContract::LongBool result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->isTaskFinished(result));
				return result;
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::ICotask::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL isTaskFinished(Antilatency::InterfaceContract::LongBool& result) {
					try {
						result = this->_object->isTaskFinished();
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
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		std::string getPropertyKey(uint32_t index) {
			throw std::logic_error{"Method IPropertyCotask.getPropertyKey() is not implemented."};
		}
		std::string getStringProperty(const std::string& key) {
			throw std::logic_error{"Method IPropertyCotask.getStringProperty() is not implemented."};
		}
		void setStringProperty(const std::string& key, const std::string& value) {
			throw std::logic_error{"Method IPropertyCotask.setStringProperty() is not implemented."};
		}
		std::vector<uint8_t> getBinaryProperty(const std::string& key) {
			throw std::logic_error{"Method IPropertyCotask.getBinaryProperty() is not implemented."};
		}
		void setBinaryProperty(const std::string& key, const std::vector<uint8_t>& value) {
			throw std::logic_error{"Method IPropertyCotask.setBinaryProperty() is not implemented."};
		}
		void deleteProperty(const std::string& key) {
			throw std::logic_error{"Method IPropertyCotask.deleteProperty() is not implemented."};
		}
		*/
		struct IPropertyCotask : Antilatency::DeviceNetwork::ICotask {
			struct VMT : Antilatency::DeviceNetwork::ICotask::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getPropertyKey(uint32_t index, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getStringProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setStringProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate value) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getBinaryProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setBinaryProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate value) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL deleteProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0x81ea9312,0xf66e,0x4708,{0xac,0xd1,0xd4,0x0a,0x3e,0x6e,0xf9,0xaa}};
				}
			private:
				~VMT() = delete;
			};
			IPropertyCotask() = default;
			IPropertyCotask(std::nullptr_t) {}
			explicit IPropertyCotask(VMT* pointer) : Antilatency::DeviceNetwork::ICotask(pointer) {}
			template<typename T, typename = typename std::enable_if<std::is_base_of<IPropertyCotask, T>::value>::type>
			IPropertyCotask& operator = (const T& other) {
			    Antilatency::DeviceNetwork::ICotask::operator=(other);
			    return *this;
			}
			template<class Implementer, class ... TArgs>
			static IPropertyCotask create(TArgs&&... args) {
			    return *new Implementer(std::forward<TArgs>(args)...);
			}
			void attach(VMT* other) ANTILATENCY_NOEXCEPT {
			    Antilatency::DeviceNetwork::ICotask::attach(other);
			}
			VMT* detach() ANTILATENCY_NOEXCEPT {
			    return reinterpret_cast<VMT*>(Antilatency::DeviceNetwork::ICotask::detach());
			}
			std::string getPropertyKey(uint32_t index) {
				std::string result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getPropertyKey(index, resultMarshaler));
				return result;
			}
			std::string getStringProperty(const std::string& key) {
				std::string result;
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getStringProperty(keyMarshaler, resultMarshaler));
				return result;
			}
			void setStringProperty(const std::string& key, const std::string& value) {
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				auto valueMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(value);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->setStringProperty(keyMarshaler, valueMarshaler));
			}
			std::vector<uint8_t> getBinaryProperty(const std::string& key) {
				std::vector<uint8_t> result;
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getBinaryProperty(keyMarshaler, resultMarshaler));
				return result;
			}
			void setBinaryProperty(const std::string& key, const std::vector<uint8_t>& value) {
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				auto valueMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(value);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->setBinaryProperty(keyMarshaler, valueMarshaler));
			}
			void deleteProperty(const std::string& key) {
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->deleteProperty(keyMarshaler));
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::IPropertyCotask, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::IPropertyCotask::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::DeviceNetwork::ICotask, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getPropertyKey(uint32_t index, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getPropertyKey(index);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getStringProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getStringProperty(key);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setStringProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate value) {
					try {
						this->_object->setStringProperty(key, value);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getBinaryProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getBinaryProperty(key);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setBinaryProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate value) {
					try {
						this->_object->setBinaryProperty(key, value);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL deleteProperty(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key) {
					try {
						this->_object->deleteProperty(key);
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
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		uint32_t getUpdateId() {
			throw std::logic_error{"Method INetwork.getUpdateId() is not implemented."};
		}
		Antilatency::DeviceNetwork::IDeviceFilter getDeviceFilter() {
			throw std::logic_error{"Method INetwork.getDeviceFilter() is not implemented."};
		}
		std::vector<Antilatency::DeviceNetwork::NodeHandle> getNodes() {
			throw std::logic_error{"Method INetwork.getNodes() is not implemented."};
		}
		Antilatency::DeviceNetwork::NodeStatus nodeGetStatus(Antilatency::DeviceNetwork::NodeHandle node) {
			throw std::logic_error{"Method INetwork.nodeGetStatus() is not implemented."};
		}
		Antilatency::InterfaceContract::LongBool nodeIsTaskSupported(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId) {
			throw std::logic_error{"Method INetwork.nodeIsTaskSupported() is not implemented."};
		}
		Antilatency::DeviceNetwork::NodeHandle nodeGetParent(Antilatency::DeviceNetwork::NodeHandle node) {
			throw std::logic_error{"Method INetwork.nodeGetParent() is not implemented."};
		}
		std::string nodeGetPhysicalPath(Antilatency::DeviceNetwork::NodeHandle node) {
			throw std::logic_error{"Method INetwork.nodeGetPhysicalPath() is not implemented."};
		}
		Antilatency::DeviceNetwork::Interop::IDataReceiver nodeStartTask(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::DeviceNetwork::Interop::IDataReceiver taskDataReceiver) {
			throw std::logic_error{"Method INetwork.nodeStartTask() is not implemented."};
		}
		Antilatency::DeviceNetwork::ISynchronousConnection nodeStartTask2(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId) {
			throw std::logic_error{"Method INetwork.nodeStartTask2() is not implemented."};
		}
		std::string nodeGetStringProperty(Antilatency::DeviceNetwork::NodeHandle node, const std::string& key) {
			throw std::logic_error{"Method INetwork.nodeGetStringProperty() is not implemented."};
		}
		std::vector<uint8_t> nodeGetBinaryProperty(Antilatency::DeviceNetwork::NodeHandle node, const std::string& key) {
			throw std::logic_error{"Method INetwork.nodeGetBinaryProperty() is not implemented."};
		}
		Antilatency::DeviceNetwork::IPropertyCotask nodeStartPropertyTask(Antilatency::DeviceNetwork::NodeHandle node) {
			throw std::logic_error{"Method INetwork.nodeStartPropertyTask() is not implemented."};
		}
		*/
		struct INetwork : Antilatency::InterfaceContract::IInterface {
			struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getUpdateId(uint32_t& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getDeviceFilter(Antilatency::DeviceNetwork::IDeviceFilter& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getNodes(Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetStatus(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::DeviceNetwork::NodeStatus& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeIsTaskSupported(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::InterfaceContract::LongBool& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetParent(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::DeviceNetwork::NodeHandle& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetPhysicalPath(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeStartTask(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::DeviceNetwork::Interop::IDataReceiver::VMT* taskDataReceiver, Antilatency::DeviceNetwork::Interop::IDataReceiver& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeStartTask2(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::DeviceNetwork::ISynchronousConnection& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetStringProperty(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetBinaryProperty(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeStartPropertyTask(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::DeviceNetwork::IPropertyCotask& result) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0x4cb2369c,0x7a66,0x4e85,{0x9a,0x0c,0xdb,0xc8,0x9f,0xc1,0xc7,0x5e}};
				}
			private:
				~VMT() = delete;
			};
			INetwork() = default;
			INetwork(std::nullptr_t) {}
			explicit INetwork(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
			template<typename T, typename = typename std::enable_if<std::is_base_of<INetwork, T>::value>::type>
			INetwork& operator = (const T& other) {
			    Antilatency::InterfaceContract::IInterface::operator=(other);
			    return *this;
			}
			template<class Implementer, class ... TArgs>
			static INetwork create(TArgs&&... args) {
			    return *new Implementer(std::forward<TArgs>(args)...);
			}
			void attach(VMT* other) ANTILATENCY_NOEXCEPT {
			    Antilatency::InterfaceContract::IInterface::attach(other);
			}
			VMT* detach() ANTILATENCY_NOEXCEPT {
			    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
			}
			uint32_t getUpdateId() {
				uint32_t result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getUpdateId(result));
				return result;
			}
			Antilatency::DeviceNetwork::IDeviceFilter getDeviceFilter() {
				Antilatency::DeviceNetwork::IDeviceFilter result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getDeviceFilter(result));
				return result;
			}
			std::vector<Antilatency::DeviceNetwork::NodeHandle> getNodes() {
				std::vector<Antilatency::DeviceNetwork::NodeHandle> result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getNodes(resultMarshaler));
				return result;
			}
			Antilatency::DeviceNetwork::NodeStatus nodeGetStatus(Antilatency::DeviceNetwork::NodeHandle node) {
				Antilatency::DeviceNetwork::NodeStatus result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeGetStatus(node, result));
				return result;
			}
			Antilatency::InterfaceContract::LongBool nodeIsTaskSupported(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId) {
				Antilatency::InterfaceContract::LongBool result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeIsTaskSupported(node, taskId, result));
				return result;
			}
			Antilatency::DeviceNetwork::NodeHandle nodeGetParent(Antilatency::DeviceNetwork::NodeHandle node) {
				Antilatency::DeviceNetwork::NodeHandle result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeGetParent(node, result));
				return result;
			}
			std::string nodeGetPhysicalPath(Antilatency::DeviceNetwork::NodeHandle node) {
				std::string result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeGetPhysicalPath(node, resultMarshaler));
				return result;
			}
			Antilatency::DeviceNetwork::Interop::IDataReceiver nodeStartTask(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::DeviceNetwork::Interop::IDataReceiver taskDataReceiver) {
				Antilatency::DeviceNetwork::Interop::IDataReceiver result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeStartTask(node, taskId, taskDataReceiver.detach(), result));
				return result;
			}
			Antilatency::DeviceNetwork::ISynchronousConnection nodeStartTask2(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId) {
				Antilatency::DeviceNetwork::ISynchronousConnection result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeStartTask2(node, taskId, result));
				return result;
			}
			std::string nodeGetStringProperty(Antilatency::DeviceNetwork::NodeHandle node, const std::string& key) {
				std::string result;
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeGetStringProperty(node, keyMarshaler, resultMarshaler));
				return result;
			}
			std::vector<uint8_t> nodeGetBinaryProperty(Antilatency::DeviceNetwork::NodeHandle node, const std::string& key) {
				std::vector<uint8_t> result;
				auto keyMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(key);
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeGetBinaryProperty(node, keyMarshaler, resultMarshaler));
				return result;
			}
			Antilatency::DeviceNetwork::IPropertyCotask nodeStartPropertyTask(Antilatency::DeviceNetwork::NodeHandle node) {
				Antilatency::DeviceNetwork::IPropertyCotask result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->nodeStartPropertyTask(node, result));
				return result;
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::INetwork, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::INetwork::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getUpdateId(uint32_t& result) {
					try {
						result = this->_object->getUpdateId();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getDeviceFilter(Antilatency::DeviceNetwork::IDeviceFilter& result) {
					try {
						result = this->_object->getDeviceFilter();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getNodes(Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getNodes();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetStatus(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::DeviceNetwork::NodeStatus& result) {
					try {
						result = this->_object->nodeGetStatus(node);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeIsTaskSupported(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::InterfaceContract::LongBool& result) {
					try {
						result = this->_object->nodeIsTaskSupported(node, taskId);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetParent(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::DeviceNetwork::NodeHandle& result) {
					try {
						result = this->_object->nodeGetParent(node);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetPhysicalPath(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->nodeGetPhysicalPath(node);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeStartTask(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::DeviceNetwork::Interop::IDataReceiver::VMT* taskDataReceiver, Antilatency::DeviceNetwork::Interop::IDataReceiver& result) {
					try {
						Antilatency::DeviceNetwork::Interop::IDataReceiver taskDataReceiverMarshaler;
						taskDataReceiverMarshaler.attach(taskDataReceiver);
						result = this->_object->nodeStartTask(node, taskId, taskDataReceiverMarshaler);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeStartTask2(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::InterfaceID taskId, Antilatency::DeviceNetwork::ISynchronousConnection& result) {
					try {
						result = this->_object->nodeStartTask2(node, taskId);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetStringProperty(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->nodeGetStringProperty(node, key);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeGetBinaryProperty(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate key, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->nodeGetBinaryProperty(node, key);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL nodeStartPropertyTask(Antilatency::DeviceNetwork::NodeHandle node, Antilatency::DeviceNetwork::IPropertyCotask& result) {
					try {
						result = this->_object->nodeStartPropertyTask(node);
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
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		Antilatency::InterfaceContract::LongBool isSupported(Antilatency::DeviceNetwork::INetwork network, Antilatency::DeviceNetwork::NodeHandle node) {
			throw std::logic_error{"Method ICotaskConstructor.isSupported() is not implemented."};
		}
		std::vector<Antilatency::DeviceNetwork::NodeHandle> findSupportedNodes(Antilatency::DeviceNetwork::INetwork network) {
			throw std::logic_error{"Method ICotaskConstructor.findSupportedNodes() is not implemented."};
		}
		*/
		struct ICotaskConstructor : Antilatency::InterfaceContract::IInterface {
			struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL isSupported(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::LongBool& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL findSupportedNodes(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0x9ecfa909,0xd13c,0x4c29,{0xa8,0x7e,0x89,0x25,0xb7,0x84,0x66,0x38}};
				}
			private:
				~VMT() = delete;
			};
			ICotaskConstructor() = default;
			ICotaskConstructor(std::nullptr_t) {}
			explicit ICotaskConstructor(VMT* pointer) : Antilatency::InterfaceContract::IInterface(pointer) {}
			template<typename T, typename = typename std::enable_if<std::is_base_of<ICotaskConstructor, T>::value>::type>
			ICotaskConstructor& operator = (const T& other) {
			    Antilatency::InterfaceContract::IInterface::operator=(other);
			    return *this;
			}
			template<class Implementer, class ... TArgs>
			static ICotaskConstructor create(TArgs&&... args) {
			    return *new Implementer(std::forward<TArgs>(args)...);
			}
			void attach(VMT* other) ANTILATENCY_NOEXCEPT {
			    Antilatency::InterfaceContract::IInterface::attach(other);
			}
			VMT* detach() ANTILATENCY_NOEXCEPT {
			    return reinterpret_cast<VMT*>(Antilatency::InterfaceContract::IInterface::detach());
			}
			Antilatency::InterfaceContract::LongBool isSupported(Antilatency::DeviceNetwork::INetwork network, Antilatency::DeviceNetwork::NodeHandle node) {
				Antilatency::InterfaceContract::LongBool result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->isSupported(network.detach(), node, result));
				return result;
			}
			std::vector<Antilatency::DeviceNetwork::NodeHandle> findSupportedNodes(Antilatency::DeviceNetwork::INetwork network) {
				std::vector<Antilatency::DeviceNetwork::NodeHandle> result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->findSupportedNodes(network.detach(), resultMarshaler));
				return result;
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::ICotaskConstructor, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::ICotaskConstructor::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL isSupported(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::DeviceNetwork::NodeHandle node, Antilatency::InterfaceContract::LongBool& result) {
					try {
						Antilatency::DeviceNetwork::INetwork networkMarshaler;
						networkMarshaler.attach(network);
						result = this->_object->isSupported(networkMarshaler, node);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL findSupportedNodes(Antilatency::DeviceNetwork::INetwork::VMT* network, Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						Antilatency::DeviceNetwork::INetwork networkMarshaler;
						networkMarshaler.attach(network);
						result = this->_object->findSupportedNodes(networkMarshaler);
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
	namespace DeviceNetwork {
		enum class LogLevel : int32_t {
			Trace = 0x0,
			Debug = 0x1,
			Info = 0x2,
			Warning = 0x3,
			Error = 0x4,
			Critical = 0x5,
			Off = 0x6
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	inline const char* enumToString(const Antilatency::DeviceNetwork::LogLevel& x) {
		switch (x) {
			case Antilatency::DeviceNetwork::LogLevel::Trace: return "Trace";
			case Antilatency::DeviceNetwork::LogLevel::Debug: return "Debug";
			case Antilatency::DeviceNetwork::LogLevel::Info: return "Info";
			case Antilatency::DeviceNetwork::LogLevel::Warning: return "Warning";
			case Antilatency::DeviceNetwork::LogLevel::Error: return "Error";
			case Antilatency::DeviceNetwork::LogLevel::Critical: return "Critical";
			case Antilatency::DeviceNetwork::LogLevel::Off: return "Off";
		}
		return "";
	}
} //namespace Antilatency

namespace Antilatency {
	namespace DeviceNetwork {
		/* copy and paste this to implementer
		Antilatency::DeviceNetwork::IDeviceFilter createFilter() {
			throw std::logic_error{"Method ILibrary.createFilter() is not implemented."};
		}
		Antilatency::DeviceNetwork::INetwork createNetwork(Antilatency::DeviceNetwork::IDeviceFilter deviceFilter) {
			throw std::logic_error{"Method ILibrary.createNetwork() is not implemented."};
		}
		std::string getVersion() {
			throw std::logic_error{"Method ILibrary.getVersion() is not implemented."};
		}
		void setLogLevel(Antilatency::DeviceNetwork::LogLevel level) {
			throw std::logic_error{"Method ILibrary.setLogLevel() is not implemented."};
		}
		*/
		struct ILibrary : Antilatency::InterfaceContract::IInterface {
			struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createFilter(Antilatency::DeviceNetwork::IDeviceFilter& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createNetwork(Antilatency::DeviceNetwork::IDeviceFilter::VMT* deviceFilter, Antilatency::DeviceNetwork::INetwork& result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getVersion(Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) = 0;
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setLogLevel(Antilatency::DeviceNetwork::LogLevel level) = 0;
				static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
					return Antilatency::InterfaceContract::InterfaceID{0xa5b7a3b0,0xda59,0x4726,{0x9b,0xa2,0x36,0xd7,0x71,0xdc,0xde,0x8a}};
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
			Antilatency::DeviceNetwork::IDeviceFilter createFilter() {
				Antilatency::DeviceNetwork::IDeviceFilter result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->createFilter(result));
				return result;
			}
			Antilatency::DeviceNetwork::INetwork createNetwork(Antilatency::DeviceNetwork::IDeviceFilter deviceFilter) {
				Antilatency::DeviceNetwork::INetwork result;
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->createNetwork(deviceFilter.detach(), result));
				return result;
			}
			std::string getVersion() {
				std::string result;
				auto resultMarshaler = Antilatency::InterfaceContract::Details::ArrayOutMarshaler::create(result);
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->getVersion(resultMarshaler));
				return result;
			}
			void setLogLevel(Antilatency::DeviceNetwork::LogLevel level) {
				handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->setLogLevel(level));
			}
		};
	} //namespace DeviceNetwork
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::DeviceNetwork::ILibrary, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::DeviceNetwork::ILibrary::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createFilter(Antilatency::DeviceNetwork::IDeviceFilter& result) {
					try {
						result = this->_object->createFilter();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createNetwork(Antilatency::DeviceNetwork::IDeviceFilter::VMT* deviceFilter, Antilatency::DeviceNetwork::INetwork& result) {
					try {
						Antilatency::DeviceNetwork::IDeviceFilter deviceFilterMarshaler;
						deviceFilterMarshaler.attach(deviceFilter);
						result = this->_object->createNetwork(deviceFilterMarshaler);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL getVersion(Antilatency::InterfaceContract::Details::ArrayOutMarshaler::Intermediate result) {
					try {
						result = this->_object->getVersion();
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL setLogLevel(Antilatency::DeviceNetwork::LogLevel level) {
					try {
						this->_object->setLogLevel(level);
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
	namespace DeviceNetwork {
		namespace Constants {
			const Antilatency::DeviceNetwork::UsbDeviceFilter AllUsbDevices = ([](){
				Antilatency::DeviceNetwork::UsbDeviceFilter result;
				(*reinterpret_cast<std::array<uint8_t, 6>*>(&result)) = {{55U, 50U, 0U, 0U, 0U, 240U}};
				return result;
			})();
			static constexpr const char* AllIpDevicesIp = "0.0.0.0";
			static constexpr const char* AllIpDevicesMask = "0.0.0.0";
			constexpr uint16_t UsbAllDeviceMask = 61440U;
			constexpr uint16_t UsbOneDeviceMask = 65535U;
		} //namespace Constants
	} //namespace DeviceNetwork
} //namespace Antilatency


#ifdef _MSC_VER
#pragma warning(pop)
#endif
