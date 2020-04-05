namespace zForceProxyCSharp
{
    using System;
    using System.Threading.Tasks;
    using Internal;

    /// <summary>
    ///     zForce sensor device
    /// </summary>
    public sealed class ZForce : IDisposable
    {
        /// <summary>
        ///     Called when a touch event is received
        /// </summary>
        /// <param name="touchEvent">Received event</param>
        public delegate void MessageReceived(TouchEvent touchEvent);

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
        /// <returns>Task on success, null on failure</returns>
        public Task<ZForceReturnCode> StartDevice(MessageReceived callback)
        {
            var ret = NativeMethods.Connect();
            if (ret != ZForceReturnCode.Ok)
            {
                return Task.FromResult(ret);
            }

            ret = NativeMethods.Configure();
            if (ret != ZForceReturnCode.Ok)
            {
                return Task.FromResult(ret);
            }

            return Task.Run(() => MessageLoop(callback));
        }

        /// <summary>
        ///     Read and process messages forever
        /// </summary>
        private ZForceReturnCode MessageLoop(MessageReceived callback)
        {
            _running = true;
            while (_running)
            {
                var touchEvent = NativeMethods.ProcessNextMessage();
                if (touchEvent.IsValid)
                {
                    callback?.Invoke(touchEvent);
                }
            }

            return ZForceReturnCode.Ok;
        }
    }
}