namespace zForceProxyCSharp
{
    /// <summary>
    ///     zForce proxy library return codes
    /// </summary>
    public enum ZForceReturnCode
    {
        Ok,
        InitFailed,
        AlreadyConnected,
        ConnectionError,
        ConfigurationError,
        MessageReadError,
        NoMessage,
        OperationTimeout,
        DeviceNotConnected
    }
}