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

        /// <summary>
        /// Called when a touch event is received
        /// </summary>
        /// <param name="touchEvent">Received event</param>
        public delegate void OnMessageReceived(TouchEvent touchEvent);

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
        public Task StartDevice(OnMessageReceived callback)
        {
            if (NativeMethods.Connect() != NativeMethods.ZForceCode.Ok)
            {
                return default;
            }

            if (NativeMethods.Configure() != NativeMethods.ZForceCode.Ok)
            {
                return default;
            }

            return Task.Run(() => MessageLoop(callback));
        }

        /// <summary>
        ///     Read and process messages forever
        /// </summary>
        private void MessageLoop(OnMessageReceived callback)
        {
            _running = true;
            while (_running)
            {
                var touchEvent = NativeMethods.ProcessNextMessage();
                if (!touchEvent.IsValid)
                {
                    _running = false;   
                }
                else
                {
                    callback?.Invoke(touchEvent);
                }
            }
        }
    }
}