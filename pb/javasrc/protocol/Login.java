// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login.proto

package protocol;

public final class Login {
  private Login() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface RegisterOrBuilder extends
      // @@protoc_insertion_point(interface_extends:Login.Register)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>required .Command.ServiceType type = 1;</code>
     */
    boolean hasType();
    /**
     * <code>required .Command.ServiceType type = 1;</code>
     */
    protocol.Command.ServiceType getType();

    /**
     * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
     */
    boolean hasChannel();
    /**
     * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
     */
    protocol.Command.ChannelID getChannel();
  }
  /**
   * Protobuf type {@code Login.Register}
   *
   * <pre>
   * 
   * </pre>
   */
  public static final class Register extends
      com.google.protobuf.GeneratedMessage implements
      // @@protoc_insertion_point(message_implements:Login.Register)
      RegisterOrBuilder {
    // Use Register.newBuilder() to construct.
    private Register(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private Register(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final Register defaultInstance;
    public static Register getDefaultInstance() {
      return defaultInstance;
    }

    public Register getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private Register(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      initFields();
      int mutable_bitField0_ = 0;
      com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder();
      try {
        boolean done = false;
        while (!done) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              done = true;
              break;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                done = true;
              }
              break;
            }
            case 8: {
              int rawValue = input.readEnum();
              protocol.Command.ServiceType value = protocol.Command.ServiceType.valueOf(rawValue);
              if (value == null) {
                unknownFields.mergeVarintField(1, rawValue);
              } else {
                bitField0_ |= 0x00000001;
                type_ = value;
              }
              break;
            }
            case 16: {
              int rawValue = input.readEnum();
              protocol.Command.ChannelID value = protocol.Command.ChannelID.valueOf(rawValue);
              if (value == null) {
                unknownFields.mergeVarintField(2, rawValue);
              } else {
                bitField0_ |= 0x00000002;
                channel_ = value;
              }
              break;
            }
          }
        }
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        throw e.setUnfinishedMessage(this);
      } catch (java.io.IOException e) {
        throw new com.google.protobuf.InvalidProtocolBufferException(
            e.getMessage()).setUnfinishedMessage(this);
      } finally {
        this.unknownFields = unknownFields.build();
        makeExtensionsImmutable();
      }
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return protocol.Login.internal_static_Login_Register_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return protocol.Login.internal_static_Login_Register_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              protocol.Login.Register.class, protocol.Login.Register.Builder.class);
    }

    public static com.google.protobuf.Parser<Register> PARSER =
        new com.google.protobuf.AbstractParser<Register>() {
      public Register parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new Register(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<Register> getParserForType() {
      return PARSER;
    }

    private int bitField0_;
    public static final int TYPE_FIELD_NUMBER = 1;
    private protocol.Command.ServiceType type_;
    /**
     * <code>required .Command.ServiceType type = 1;</code>
     */
    public boolean hasType() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required .Command.ServiceType type = 1;</code>
     */
    public protocol.Command.ServiceType getType() {
      return type_;
    }

    public static final int CHANNEL_FIELD_NUMBER = 2;
    private protocol.Command.ChannelID channel_;
    /**
     * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
     */
    public boolean hasChannel() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    /**
     * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
     */
    public protocol.Command.ChannelID getChannel() {
      return channel_;
    }

    private void initFields() {
      type_ = protocol.Command.ServiceType.Login;
      channel_ = protocol.Command.ChannelID.Normal;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized == 1) return true;
      if (isInitialized == 0) return false;

      if (!hasType()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasChannel()) {
        memoizedIsInitialized = 0;
        return false;
      }
      memoizedIsInitialized = 1;
      return true;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeEnum(1, type_.getNumber());
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeEnum(2, channel_.getNumber());
      }
      getUnknownFields().writeTo(output);
    }

    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeEnumSize(1, type_.getNumber());
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeEnumSize(2, channel_.getNumber());
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }

    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }

    public static protocol.Login.Register parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static protocol.Login.Register parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static protocol.Login.Register parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static protocol.Login.Register parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static protocol.Login.Register parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static protocol.Login.Register parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static protocol.Login.Register parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static protocol.Login.Register parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static protocol.Login.Register parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static protocol.Login.Register parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(protocol.Login.Register prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code Login.Register}
     *
     * <pre>
     * 
     * </pre>
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:Login.Register)
        protocol.Login.RegisterOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return protocol.Login.internal_static_Login_Register_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return protocol.Login.internal_static_Login_Register_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                protocol.Login.Register.class, protocol.Login.Register.Builder.class);
      }

      // Construct using protocol.Login.Register.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }

      private Builder(
          com.google.protobuf.GeneratedMessage.BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        }
      }
      private static Builder create() {
        return new Builder();
      }

      public Builder clear() {
        super.clear();
        type_ = protocol.Command.ServiceType.Login;
        bitField0_ = (bitField0_ & ~0x00000001);
        channel_ = protocol.Command.ChannelID.Normal;
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return protocol.Login.internal_static_Login_Register_descriptor;
      }

      public protocol.Login.Register getDefaultInstanceForType() {
        return protocol.Login.Register.getDefaultInstance();
      }

      public protocol.Login.Register build() {
        protocol.Login.Register result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public protocol.Login.Register buildPartial() {
        protocol.Login.Register result = new protocol.Login.Register(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.type_ = type_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.channel_ = channel_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof protocol.Login.Register) {
          return mergeFrom((protocol.Login.Register)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(protocol.Login.Register other) {
        if (other == protocol.Login.Register.getDefaultInstance()) return this;
        if (other.hasType()) {
          setType(other.getType());
        }
        if (other.hasChannel()) {
          setChannel(other.getChannel());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        if (!hasType()) {
          
          return false;
        }
        if (!hasChannel()) {
          
          return false;
        }
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        protocol.Login.Register parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (protocol.Login.Register) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      private protocol.Command.ServiceType type_ = protocol.Command.ServiceType.Login;
      /**
       * <code>required .Command.ServiceType type = 1;</code>
       */
      public boolean hasType() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      /**
       * <code>required .Command.ServiceType type = 1;</code>
       */
      public protocol.Command.ServiceType getType() {
        return type_;
      }
      /**
       * <code>required .Command.ServiceType type = 1;</code>
       */
      public Builder setType(protocol.Command.ServiceType value) {
        if (value == null) {
          throw new NullPointerException();
        }
        bitField0_ |= 0x00000001;
        type_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required .Command.ServiceType type = 1;</code>
       */
      public Builder clearType() {
        bitField0_ = (bitField0_ & ~0x00000001);
        type_ = protocol.Command.ServiceType.Login;
        onChanged();
        return this;
      }

      private protocol.Command.ChannelID channel_ = protocol.Command.ChannelID.Normal;
      /**
       * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
       */
      public boolean hasChannel() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      /**
       * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
       */
      public protocol.Command.ChannelID getChannel() {
        return channel_;
      }
      /**
       * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
       */
      public Builder setChannel(protocol.Command.ChannelID value) {
        if (value == null) {
          throw new NullPointerException();
        }
        bitField0_ |= 0x00000002;
        channel_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required .Command.ChannelID channel = 2 [default = Normal];</code>
       */
      public Builder clearChannel() {
        bitField0_ = (bitField0_ & ~0x00000002);
        channel_ = protocol.Command.ChannelID.Normal;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:Login.Register)
    }

    static {
      defaultInstance = new Register(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:Login.Register)
  }

  public interface RegisterResponseOrBuilder extends
      // @@protoc_insertion_point(interface_extends:Login.RegisterResponse)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>required .Command.ErrorCode errorCode = 1;</code>
     */
    boolean hasErrorCode();
    /**
     * <code>required .Command.ErrorCode errorCode = 1;</code>
     */
    protocol.Command.ErrorCode getErrorCode();
  }
  /**
   * Protobuf type {@code Login.RegisterResponse}
   */
  public static final class RegisterResponse extends
      com.google.protobuf.GeneratedMessage implements
      // @@protoc_insertion_point(message_implements:Login.RegisterResponse)
      RegisterResponseOrBuilder {
    // Use RegisterResponse.newBuilder() to construct.
    private RegisterResponse(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private RegisterResponse(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final RegisterResponse defaultInstance;
    public static RegisterResponse getDefaultInstance() {
      return defaultInstance;
    }

    public RegisterResponse getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private RegisterResponse(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      initFields();
      int mutable_bitField0_ = 0;
      com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder();
      try {
        boolean done = false;
        while (!done) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              done = true;
              break;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                done = true;
              }
              break;
            }
            case 8: {
              int rawValue = input.readEnum();
              protocol.Command.ErrorCode value = protocol.Command.ErrorCode.valueOf(rawValue);
              if (value == null) {
                unknownFields.mergeVarintField(1, rawValue);
              } else {
                bitField0_ |= 0x00000001;
                errorCode_ = value;
              }
              break;
            }
          }
        }
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        throw e.setUnfinishedMessage(this);
      } catch (java.io.IOException e) {
        throw new com.google.protobuf.InvalidProtocolBufferException(
            e.getMessage()).setUnfinishedMessage(this);
      } finally {
        this.unknownFields = unknownFields.build();
        makeExtensionsImmutable();
      }
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return protocol.Login.internal_static_Login_RegisterResponse_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return protocol.Login.internal_static_Login_RegisterResponse_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              protocol.Login.RegisterResponse.class, protocol.Login.RegisterResponse.Builder.class);
    }

    public static com.google.protobuf.Parser<RegisterResponse> PARSER =
        new com.google.protobuf.AbstractParser<RegisterResponse>() {
      public RegisterResponse parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new RegisterResponse(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<RegisterResponse> getParserForType() {
      return PARSER;
    }

    private int bitField0_;
    public static final int ERRORCODE_FIELD_NUMBER = 1;
    private protocol.Command.ErrorCode errorCode_;
    /**
     * <code>required .Command.ErrorCode errorCode = 1;</code>
     */
    public boolean hasErrorCode() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required .Command.ErrorCode errorCode = 1;</code>
     */
    public protocol.Command.ErrorCode getErrorCode() {
      return errorCode_;
    }

    private void initFields() {
      errorCode_ = protocol.Command.ErrorCode.Success;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized == 1) return true;
      if (isInitialized == 0) return false;

      if (!hasErrorCode()) {
        memoizedIsInitialized = 0;
        return false;
      }
      memoizedIsInitialized = 1;
      return true;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeEnum(1, errorCode_.getNumber());
      }
      getUnknownFields().writeTo(output);
    }

    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeEnumSize(1, errorCode_.getNumber());
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }

    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }

    public static protocol.Login.RegisterResponse parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static protocol.Login.RegisterResponse parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static protocol.Login.RegisterResponse parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static protocol.Login.RegisterResponse parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static protocol.Login.RegisterResponse parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static protocol.Login.RegisterResponse parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static protocol.Login.RegisterResponse parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static protocol.Login.RegisterResponse parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static protocol.Login.RegisterResponse parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static protocol.Login.RegisterResponse parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(protocol.Login.RegisterResponse prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code Login.RegisterResponse}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:Login.RegisterResponse)
        protocol.Login.RegisterResponseOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return protocol.Login.internal_static_Login_RegisterResponse_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return protocol.Login.internal_static_Login_RegisterResponse_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                protocol.Login.RegisterResponse.class, protocol.Login.RegisterResponse.Builder.class);
      }

      // Construct using protocol.Login.RegisterResponse.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }

      private Builder(
          com.google.protobuf.GeneratedMessage.BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        }
      }
      private static Builder create() {
        return new Builder();
      }

      public Builder clear() {
        super.clear();
        errorCode_ = protocol.Command.ErrorCode.Success;
        bitField0_ = (bitField0_ & ~0x00000001);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return protocol.Login.internal_static_Login_RegisterResponse_descriptor;
      }

      public protocol.Login.RegisterResponse getDefaultInstanceForType() {
        return protocol.Login.RegisterResponse.getDefaultInstance();
      }

      public protocol.Login.RegisterResponse build() {
        protocol.Login.RegisterResponse result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public protocol.Login.RegisterResponse buildPartial() {
        protocol.Login.RegisterResponse result = new protocol.Login.RegisterResponse(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.errorCode_ = errorCode_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof protocol.Login.RegisterResponse) {
          return mergeFrom((protocol.Login.RegisterResponse)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(protocol.Login.RegisterResponse other) {
        if (other == protocol.Login.RegisterResponse.getDefaultInstance()) return this;
        if (other.hasErrorCode()) {
          setErrorCode(other.getErrorCode());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        if (!hasErrorCode()) {
          
          return false;
        }
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        protocol.Login.RegisterResponse parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (protocol.Login.RegisterResponse) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      private protocol.Command.ErrorCode errorCode_ = protocol.Command.ErrorCode.Success;
      /**
       * <code>required .Command.ErrorCode errorCode = 1;</code>
       */
      public boolean hasErrorCode() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      /**
       * <code>required .Command.ErrorCode errorCode = 1;</code>
       */
      public protocol.Command.ErrorCode getErrorCode() {
        return errorCode_;
      }
      /**
       * <code>required .Command.ErrorCode errorCode = 1;</code>
       */
      public Builder setErrorCode(protocol.Command.ErrorCode value) {
        if (value == null) {
          throw new NullPointerException();
        }
        bitField0_ |= 0x00000001;
        errorCode_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required .Command.ErrorCode errorCode = 1;</code>
       */
      public Builder clearErrorCode() {
        bitField0_ = (bitField0_ & ~0x00000001);
        errorCode_ = protocol.Command.ErrorCode.Success;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:Login.RegisterResponse)
    }

    static {
      defaultInstance = new RegisterResponse(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:Login.RegisterResponse)
  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_Login_Register_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_Login_Register_fieldAccessorTable;
  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_Login_RegisterResponse_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_Login_RegisterResponse_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\013login.proto\022\005Login\032\rcommand.proto\"[\n\010R" +
      "egister\022\"\n\004type\030\001 \002(\0162\024.Command.ServiceT" +
      "ype\022+\n\007channel\030\002 \002(\0162\022.Command.ChannelID" +
      ":\006Normal\"9\n\020RegisterResponse\022%\n\terrorCod" +
      "e\030\001 \002(\0162\022.Command.ErrorCodeB\021\n\010protocolB" +
      "\005Login"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
        new com.google.protobuf.Descriptors.FileDescriptor.    InternalDescriptorAssigner() {
          public com.google.protobuf.ExtensionRegistry assignDescriptors(
              com.google.protobuf.Descriptors.FileDescriptor root) {
            descriptor = root;
            return null;
          }
        };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          protocol.Command.getDescriptor(),
        }, assigner);
    internal_static_Login_Register_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_Login_Register_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessage.FieldAccessorTable(
        internal_static_Login_Register_descriptor,
        new java.lang.String[] { "Type", "Channel", });
    internal_static_Login_RegisterResponse_descriptor =
      getDescriptor().getMessageTypes().get(1);
    internal_static_Login_RegisterResponse_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessage.FieldAccessorTable(
        internal_static_Login_RegisterResponse_descriptor,
        new java.lang.String[] { "ErrorCode", });
    protocol.Command.getDescriptor();
  }

  // @@protoc_insertion_point(outer_class_scope)
}
