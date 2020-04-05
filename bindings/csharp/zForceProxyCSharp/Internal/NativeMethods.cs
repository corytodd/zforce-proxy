namespace zForceProxyCSharp.Internal
{
    using System.Drawing;
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
            var ret = zforce_configure();
            return (ZForceCode) ret;
        }

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        private static extern int zforce_process_next_message(int filter, ref ZTouchEvent touchEvent);

        /// <summary>
        ///     Process next available message, if any
        /// </summary>
        /// <returns></returns>
        public static TouchEvent ProcessNextMessage()
        {
            var touchEvent = new ZTouchEvent();
            var ret = (ZForceCode) zforce_process_next_message(int.MaxValue, ref touchEvent);

            if (ret != ZForceCode.Ok)
            {
                return default;
            }

            Size? size = null;
            if (touchEvent.hasSizeX || touchEvent.hasSizeY)
            {
                size = new Size((int) touchEvent.sizeX, (int) touchEvent.sizeY);
            }

            Point? position = null;
            if (touchEvent.hasSizeX || touchEvent.hasSizeY)
            {
                position = new Point((int) touchEvent.X, (int) touchEvent.Y);
            }

            return new TouchEvent(touchEvent.eventType, size, position);
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
            MessageReadError,
            NoMessage,
            OperationTimeout
        }


        [StructLayout(LayoutKind.Sequential, Pack = 2)]
        private struct ZTouchEvent
        {
            /// <summary>
            ///     True if X has a value
            /// </summary>
            [MarshalAs(UnmanagedType.I1)]
            public readonly bool hasX;

            /// <summary>
            ///     True if sizeX has a value
            /// </summary>
            [MarshalAs(UnmanagedType.I1)]
            public readonly bool hasSizeX;

            /// <summary>
            ///     True if Y has a value
            /// </summary>
            [MarshalAs(UnmanagedType.I1)]
            public readonly bool hasY;

            /// <summary>
            ///     True if sizeY has a value
            /// </summary>
            [MarshalAs(UnmanagedType.I1)]
            public readonly bool hasSizeY;

            /// <summary>
            ///     True if Z has a value
            /// </summary>
            [MarshalAs(UnmanagedType.I1)]
            public readonly bool hasZ;

            /// <summary>
            ///     True if sizeZ has a value
            /// </summary>
            [MarshalAs(UnmanagedType.I1)]
            private readonly bool hasSizeZ;

            /// <summary>
            ///     Type of touch event
            /// </summary>
            public readonly ZEventType eventType;

            /// <summary>
            ///     Id of this event
            /// </summary>
            public readonly uint id;

            /// <summary>
            ///     Size of X touch
            /// </summary>
            public readonly uint sizeX;

            /// <summary>
            ///     Size of Y touch
            /// </summary>
            public readonly uint sizeY;

            /// <summary>
            ///     Size of Z touch
            /// </summary>
            public readonly uint sizeZ;

            /// <summary>
            ///     X coordinate
            /// </summary>
            public readonly uint X;

            /// <summary>
            ///     Y coordinate
            /// </summary>
            public readonly uint Y;

            /// <summary>
            ///     Z coordinate
            /// </summary>
            public readonly uint Z;
        }
    }
}