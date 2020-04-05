namespace zForceProxyCSharp.CLI
{
    using System.Runtime.InteropServices;

    public static class ConsoleInterrupt
    {
        [DllImport("Kernel32")]
        public static extern bool SetConsoleCtrlHandler(HandlerRoutine handler, bool add);
        
        public delegate bool HandlerRoutine(CtrlTypes ctrlType);
        
        public enum CtrlTypes
        {
            CtrlCEvent = 0,
            CtrlBreakEvent,
            CtrlCloseEvent,
            CtrlLogoffEvent = 5,
            CtrlShutdownEvent
        }
    }
}