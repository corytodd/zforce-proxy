namespace zForceProxyCSharp.Internal
{
    using System.Runtime.InteropServices;

    /// <summary>
    ///     Bindings for native functions exported from zforce_proxy library
    /// </summary>
    internal static class NativeMethods
    {
        private const string DllName =
#if DEBUG
            "zforce_proxyd";
#else
            "zforce_proxy";
#endif
        [DllImport(DllName, CharSet = CharSet.Auto)]
        private static extern int zforce_initialize();

        /// <summary>
        ///     Initialize this library
        /// </summary>
        public static ZForceCode Initialize()
        {
            var ret = zforce_initialize();
            return (ZForceCode) ret;
        }

        [DllImport(DllName, CharSet = CharSet.Auto)]
        private static extern int zforce_connect();

        /// <summary>
        ///     Create a new connection
        /// </summary>
        public static ZForceCode Connect()
        {
            var ret = zforce_connect();
            return (ZForceCode) ret;
        }

        [DllImport(DllName, CharSet = CharSet.Auto)]
        private static extern int zforce_configure();

        /// <summary>
        ///     Configure the current connection
        /// </summary>
        /// <returns></returns>
        public static ZForceCode Configure()
        {
            var ret = zforce_connect();
            return (ZForceCode) ret;
        }

        [DllImport(DllName, CharSet = CharSet.Auto)]
        private static extern int zforce_process_next_message();

        /// <summary>
        ///     Process next available message, if any
        /// </summary>
        /// <returns></returns>
        public static ZForceCode ProcessNextMessage()
        {
            var ret = zforce_connect();
            return (ZForceCode) ret;
        }

        [DllImport(DllName, CharSet = CharSet.Auto)]
        private static extern void zforce_deinitialize();

        public static void Deinitialize()
        {
            zforce_deinitialize();
        }

        [DllImport(DllName, CharSet = CharSet.Auto)]
        private static extern void zforce_get_version(ref int version);

        public static ZForceVersion GetVersion()
        {
            var version = 0;
            zforce_get_version(ref version);

            return new ZForceVersion(version);
        }

        /// <summary>
        ///     zForce proxy library return codes
        /// </summary>
        internal enum ZForceCode
        {
            Ok,
            InitFailed,
            AlreadyConnected,
            ConnectionError,
            ConfigurationError,
            MessageReadError
        }
    }
}