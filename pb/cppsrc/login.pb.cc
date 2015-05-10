// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "login.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace Login {

namespace {

const ::google::protobuf::Descriptor* Register_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Register_reflection_ = NULL;
const ::google::protobuf::Descriptor* RegisterResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  RegisterResponse_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_login_2eproto() {
  protobuf_AddDesc_login_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "login.proto");
  GOOGLE_CHECK(file != NULL);
  Register_descriptor_ = file->message_type(0);
  static const int Register_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Register, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Register, channel_),
  };
  Register_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Register_descriptor_,
      Register::default_instance_,
      Register_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Register, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Register, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Register));
  RegisterResponse_descriptor_ = file->message_type(1);
  static const int RegisterResponse_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RegisterResponse, errorcode_),
  };
  RegisterResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      RegisterResponse_descriptor_,
      RegisterResponse::default_instance_,
      RegisterResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RegisterResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RegisterResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(RegisterResponse));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_login_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Register_descriptor_, &Register::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    RegisterResponse_descriptor_, &RegisterResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_login_2eproto() {
  delete Register::default_instance_;
  delete Register_reflection_;
  delete RegisterResponse::default_instance_;
  delete RegisterResponse_reflection_;
}

void protobuf_AddDesc_login_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::Command::protobuf_AddDesc_command_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013login.proto\022\005Login\032\rcommand.proto\"[\n\010R"
    "egister\022\"\n\004type\030\001 \002(\0162\024.Command.ServiceT"
    "ype\022+\n\007channel\030\002 \002(\0162\022.Command.ChannelID"
    ":\006Normal\"9\n\020RegisterResponse\022%\n\terrorCod"
    "e\030\001 \002(\0162\022.Command.ErrorCodeB\021\n\010protocolB"
    "\005Login", 206);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "login.proto", &protobuf_RegisterTypes);
  Register::default_instance_ = new Register();
  RegisterResponse::default_instance_ = new RegisterResponse();
  Register::default_instance_->InitAsDefaultInstance();
  RegisterResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_login_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_login_2eproto {
  StaticDescriptorInitializer_login_2eproto() {
    protobuf_AddDesc_login_2eproto();
  }
} static_descriptor_initializer_login_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Register::kTypeFieldNumber;
const int Register::kChannelFieldNumber;
#endif  // !_MSC_VER

Register::Register()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Register::InitAsDefaultInstance() {
}

Register::Register(const Register& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Register::SharedCtor() {
  _cached_size_ = 0;
  type_ = 1;
  channel_ = 1;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Register::~Register() {
  SharedDtor();
}

void Register::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Register::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Register::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Register_descriptor_;
}

const Register& Register::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_login_2eproto();
  return *default_instance_;
}

Register* Register::default_instance_ = NULL;

Register* Register::New() const {
  return new Register;
}

void Register::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 1;
    channel_ = 1;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Register::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .Command.ServiceType type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Command::ServiceType_IsValid(value)) {
            set_type(static_cast< ::Command::ServiceType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_channel;
        break;
      }

      // required .Command.ChannelID channel = 2 [default = Normal];
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_channel:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Command::ChannelID_IsValid(value)) {
            set_channel(static_cast< ::Command::ChannelID >(value));
          } else {
            mutable_unknown_fields()->AddVarint(2, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Register::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .Command.ServiceType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // required .Command.ChannelID channel = 2 [default = Normal];
  if (has_channel()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->channel(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Register::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .Command.ServiceType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // required .Command.ChannelID channel = 2 [default = Normal];
  if (has_channel()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      2, this->channel(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Register::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .Command.ServiceType type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

    // required .Command.ChannelID channel = 2 [default = Normal];
    if (has_channel()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->channel());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Register::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Register* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Register*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Register::MergeFrom(const Register& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_channel()) {
      set_channel(from.channel());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Register::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Register::CopyFrom(const Register& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Register::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void Register::Swap(Register* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(channel_, other->channel_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Register::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Register_descriptor_;
  metadata.reflection = Register_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int RegisterResponse::kErrorCodeFieldNumber;
#endif  // !_MSC_VER

RegisterResponse::RegisterResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void RegisterResponse::InitAsDefaultInstance() {
}

RegisterResponse::RegisterResponse(const RegisterResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void RegisterResponse::SharedCtor() {
  _cached_size_ = 0;
  errorcode_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RegisterResponse::~RegisterResponse() {
  SharedDtor();
}

void RegisterResponse::SharedDtor() {
  if (this != default_instance_) {
  }
}

void RegisterResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* RegisterResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RegisterResponse_descriptor_;
}

const RegisterResponse& RegisterResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_login_2eproto();
  return *default_instance_;
}

RegisterResponse* RegisterResponse::default_instance_ = NULL;

RegisterResponse* RegisterResponse::New() const {
  return new RegisterResponse;
}

void RegisterResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    errorcode_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool RegisterResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .Command.ErrorCode errorCode = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Command::ErrorCode_IsValid(value)) {
            set_errorcode(static_cast< ::Command::ErrorCode >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void RegisterResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .Command.ErrorCode errorCode = 1;
  if (has_errorcode()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->errorcode(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* RegisterResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .Command.ErrorCode errorCode = 1;
  if (has_errorcode()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->errorcode(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int RegisterResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .Command.ErrorCode errorCode = 1;
    if (has_errorcode()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->errorcode());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RegisterResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const RegisterResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const RegisterResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void RegisterResponse::MergeFrom(const RegisterResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_errorcode()) {
      set_errorcode(from.errorcode());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void RegisterResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RegisterResponse::CopyFrom(const RegisterResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RegisterResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void RegisterResponse::Swap(RegisterResponse* other) {
  if (other != this) {
    std::swap(errorcode_, other->errorcode_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata RegisterResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = RegisterResponse_descriptor_;
  metadata.reflection = RegisterResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Login

// @@protoc_insertion_point(global_scope)
