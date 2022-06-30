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
#include <Antilatency.Alt.Environment.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4946)
#pragma warning(disable: 4265)
#endif
namespace Antilatency {
	namespace Alt {
		namespace Environment {
			namespace Selector {
				/* copy and paste this to implementer
				void registerEnvironmentProvider(const std::string& name, Antilatency::Alt::Environment::IEnvironmentConstructor provider) {
					throw std::logic_error{"Method ILibrary.registerEnvironmentProvider() is not implemented."};
				}
				void unregisterEnvironmentProvider(const std::string& name) {
					throw std::logic_error{"Method ILibrary.unregisterEnvironmentProvider() is not implemented."};
				}
				Antilatency::Alt::Environment::IEnvironment createEnvironment(const std::string& data) {
					throw std::logic_error{"Method ILibrary.createEnvironment() is not implemented."};
				}
				*/
				struct ILibrary : Antilatency::InterfaceContract::IInterface {
					struct VMT : Antilatency::InterfaceContract::IInterface::VMT {
						virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL registerEnvironmentProvider(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate name, Antilatency::Alt::Environment::IEnvironmentConstructor::VMT* provider) = 0;
						virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL unregisterEnvironmentProvider(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate name) = 0;
						virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createEnvironment(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate data, Antilatency::Alt::Environment::IEnvironment& result) = 0;
						static constexpr Antilatency::InterfaceContract::InterfaceID ID() {
							return Antilatency::InterfaceContract::InterfaceID{0x2ef251c9,0xe0d5,0x46ad,{0x8a,0xe0,0x96,0x7d,0xed,0x90,0x8f,0x2a}};
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
					void registerEnvironmentProvider(const std::string& name, Antilatency::Alt::Environment::IEnvironmentConstructor provider) {
						auto nameMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(name);
						handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->registerEnvironmentProvider(nameMarshaler, provider.detach()));
					}
					void unregisterEnvironmentProvider(const std::string& name) {
						auto nameMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(name);
						handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->unregisterEnvironmentProvider(nameMarshaler));
					}
					Antilatency::Alt::Environment::IEnvironment createEnvironment(const std::string& data) {
						Antilatency::Alt::Environment::IEnvironment result;
						auto dataMarshaler = Antilatency::InterfaceContract::Details::ArrayInMarshaler::create(data);
						handleExceptionCode((reinterpret_cast<VMT*>(this->_object))->createEnvironment(dataMarshaler, result));
						return result;
					}
				};
			} //namespace Selector
		} //namespace Environment
	} //namespace Alt
} //namespace Antilatency
namespace Antilatency {
	namespace InterfaceContract {
		namespace Details {
			template<typename Implementer, typename LifeTimeController>
			struct InterfaceRemap<Antilatency::Alt::Environment::Selector::ILibrary, Implementer, LifeTimeController> : Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController> {
			public:
			    InterfaceRemap() = default;
			    static bool isInterfaceSupported(const Antilatency::InterfaceContract::InterfaceID& id) {
					if (id == Antilatency::Alt::Environment::Selector::ILibrary::VMT::ID()) {
						return true;
					}
					return Antilatency::InterfaceContract::Details::InterfaceRemap<Antilatency::InterfaceContract::IInterface, Implementer, LifeTimeController>::isInterfaceSupported(id);
				}
			public:
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL registerEnvironmentProvider(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate name, Antilatency::Alt::Environment::IEnvironmentConstructor::VMT* provider) {
					try {
						Antilatency::Alt::Environment::IEnvironmentConstructor providerMarshaler;
						providerMarshaler.attach(provider);
						this->_object->registerEnvironmentProvider(name, providerMarshaler);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL unregisterEnvironmentProvider(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate name) {
					try {
						this->_object->unregisterEnvironmentProvider(name);
					}
					catch (...) {
						return Antilatency::InterfaceContract::handleRemapException(this->_object);
					}
					return Antilatency::InterfaceContract::ExceptionCode::Ok;
				}
				virtual Antilatency::InterfaceContract::ExceptionCode ANTILATENCY_INTERFACE_CALL createEnvironment(Antilatency::InterfaceContract::Details::ArrayInMarshaler::Intermediate data, Antilatency::Alt::Environment::IEnvironment& result) {
					try {
						result = this->_object->createEnvironment(data);
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


#ifdef _MSC_VER
#pragma warning(pop)
#endif
