# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: command.proto

from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)




DESCRIPTOR = _descriptor.FileDescriptor(
  name='command.proto',
  package='Command',
  serialized_pb='\n\rcommand.proto\x12\x07\x43ommand*\x18\n\tErrorCode\x12\x0b\n\x07Success\x10\x00*\x18\n\x0bServiceType\x12\t\n\x05Login\x10\x01*\x17\n\tChannelID\x12\n\n\x06Normal\x10\x01*8\n\tMessageID\x12\x14\n\x0fRegisterRequest\x10\x81\x02\x12\x15\n\x10RegisterResponse\x10\x82\x02\x42\x13\n\x08protocolB\x07\x43ommand')

_ERRORCODE = _descriptor.EnumDescriptor(
  name='ErrorCode',
  full_name='Command.ErrorCode',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='Success', index=0, number=0,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=26,
  serialized_end=50,
)

ErrorCode = enum_type_wrapper.EnumTypeWrapper(_ERRORCODE)
_SERVICETYPE = _descriptor.EnumDescriptor(
  name='ServiceType',
  full_name='Command.ServiceType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='Login', index=0, number=1,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=52,
  serialized_end=76,
)

ServiceType = enum_type_wrapper.EnumTypeWrapper(_SERVICETYPE)
_CHANNELID = _descriptor.EnumDescriptor(
  name='ChannelID',
  full_name='Command.ChannelID',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='Normal', index=0, number=1,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=78,
  serialized_end=101,
)

ChannelID = enum_type_wrapper.EnumTypeWrapper(_CHANNELID)
_MESSAGEID = _descriptor.EnumDescriptor(
  name='MessageID',
  full_name='Command.MessageID',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='RegisterRequest', index=0, number=257,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='RegisterResponse', index=1, number=258,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=103,
  serialized_end=159,
)

MessageID = enum_type_wrapper.EnumTypeWrapper(_MESSAGEID)
Success = 0
Login = 1
Normal = 1
RegisterRequest = 257
RegisterResponse = 258




DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), '\n\010protocolB\007Command')
# @@protoc_insertion_point(module_scope)