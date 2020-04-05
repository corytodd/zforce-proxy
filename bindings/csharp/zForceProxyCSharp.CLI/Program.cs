namespace zForceProxyCSharp.CLI
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;

    internal static class Program
    {
        private static readonly CancellationTokenSource TokenSource = new CancellationTokenSource();

        private static async Task Main(string[] args)
        {
            // Capture ctrl+c to stop process
            ConsoleInterrupt.SetConsoleCtrlHandler(ConsoleHandler, true);

            Console.WriteLine($"Using zForceProxy v: {ZForce.Instance.Version}");
            Console.WriteLine("CTRL+C to stop");

            // Run device poll as a task
            var messageLoop = ZForce.Instance.StartDevice(m => { Console.WriteLine(m); });


            // Run the task until user uses ctrl+c to abort
            try
            {
                var ret = await messageLoop;
                if (ret != ZForceReturnCode.Ok)
                {
                    Console.WriteLine($"Error: {ret.ToString()}");
                }
            }
            catch (OperationCanceledException)
            {
                // Nothing to do
            }
            
            Console.WriteLine("Shutting down");
        }

        /// <summary>
        ///     Console interrupt handler
        /// </summary>
        /// <param name="ctrl">Console control code</param>
        /// <returns>true if interrupt was handled, false otherwise</returns>
        private static bool ConsoleHandler(ConsoleInterrupt.CtrlTypes ctrl)
        {
            // Only handle ctrl+c
            if (ctrl != ConsoleInterrupt.CtrlTypes.CtrlCEvent)
            {
                return false;
            }

            // Stop the zForce task
            TokenSource.Cancel();

            // Detach this handler
            ConsoleInterrupt.SetConsoleCtrlHandler(ConsoleHandler, false);

            // Yes, we handled the interrupt
            return true;
        }
    }
}