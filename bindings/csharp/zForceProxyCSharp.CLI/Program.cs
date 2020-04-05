using System;

namespace zForceProxyCSharp.CLI
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            Console.WriteLine($"Using zForceProxy v: {ZForce.Instance.Version}");

            var messageLoop = ZForce.Instance.StartDevice((m) =>
            {
                Console.WriteLine(m);
            });

            messageLoop.Wait();
        }
    }
}