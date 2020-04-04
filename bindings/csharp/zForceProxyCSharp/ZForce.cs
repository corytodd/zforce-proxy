namespace zForceProxyCSharp
{
    using System;
    using System.Threading.Tasks;
    using Internal;

    public sealed class ZForce : IDisposable
    {
        private static readonly Lazy<ZForce> Lazy = new Lazy<ZForce>(() => new ZForce());

        private bool _running;

        private ZForce()
        {
            NativeMethods.Initialize();
        }

        /// <summary>
        ///     Get ZForce instance
        /// </summary>
        public static ZForce Instance => Lazy.Value;

        /// <summary>
        ///     Get version of zForceProxy
        /// </summary>
        public ZForceVersion Version => NativeMethods.GetVersion();

        /// <inheritdoc />
        public void Dispose()
        {
            _running = false;
            NativeMethods.Deinitialize();
        }

        /// <summary>
        ///     Start the zForce device connection
        /// </summary>
        /// <returns></returns>
        public Task StartDevice()
        {
            NativeMethods.Connect();

            NativeMethods.Configure();

            return Task.Run(MessageLoop);
        }

        /// <summary>
        ///     Read and process messages forever
        /// </summary>
        private void MessageLoop()
        {
            _running = true;
            while (_running)
            {
                NativeMethods.ProcessNextMessage();
            }
        }
    }
}