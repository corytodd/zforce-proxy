namespace zForceProxyCSharp
{
    /// <summary>
    ///     zForce proxy library return codes
    /// </summary>
    public enum ZForceReturnCode
    {
        /// <summary>
        ///     No error
        /// </summary>
        Ok,

        /// <summary>
        ///     Library initialization fails
        /// </summary>
        InitFailed,

        /// <summary>
        ///     A device is already connected
        ///     We only support a single device connection
        /// </summary>
        AlreadyConnected,

        /// <summary>
        ///     Could not connect for some reason
        /// </summary>
        ConnectionError,

        /// <summary>
        ///     Could not configure for some reason
        /// </summary>
        ConfigurationError,

        /// <summary>
        ///     Could not read a message for some reason
        /// </summary>
        MessageReadError,

        /// <summary>
        ///     No message available
        /// </summary>
        NoMessage,

        /// <summary>
        ///     Timed out reading a message
        /// </summary>
        OperationTimeout,

        /// <summary>
        ///     No device is connected
        /// </summary>
        DeviceNotConnected
    }
}