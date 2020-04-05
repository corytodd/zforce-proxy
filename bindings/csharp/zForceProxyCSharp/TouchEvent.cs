namespace zForceProxyCSharp
{
    using System.Drawing;
    using System.Text;

    /// <summary>
    ///     Touch event types
    /// </summary>
    public enum ZEventType
    {
        /// <summary>
        ///     New touch object
        /// </summary>
        Down,

        /// <summary>
        ///     Touch is moving
        /// </summary>
        Move,

        /// <summary>
        ///     Touch is no longer detected
        /// </summary>
        Up,

        /// <summary>
        ///     Invalid event reported by device
        /// </summary>
        Invalid,

        /// <summary>
        ///     Ghost touch
        /// </summary>
        Ghost
    }

    /// <summary>
    ///     2D touch event (Z is not handled)
    /// </summary>
    public readonly struct TouchEvent
    {
        /// <summary>
        ///     Create a new touch event
        /// </summary>
        /// <param name="eventType">Type event</param>
        /// <param name="size">Size of detected touch in pixels</param>
        /// <param name="logicalPoint">Logical position of touch event</param>
        public TouchEvent(ZEventType eventType, Size? size, ZPoint? logicalPoint)
        {
            IsValid = true;
            EventType = eventType;
            Size = size;
            LogicalPoint = logicalPoint;
        }

        /// <summary>
        ///     Returns true if object is valid
        /// </summary>
        public readonly bool IsValid;

        /// <summary>
        ///     Original event type
        /// </summary>
        public readonly ZEventType EventType;

        /// <summary>
        ///     Size of touch event (covered pixels)
        /// </summary>
        public readonly Size? Size;

        /// <summary>
        ///     Position of touch in cartesian coordinates
        /// </summary>
        public readonly ZPoint? LogicalPoint;

        /// <summary>
        ///     Returns [Event] Size? Position?
        /// </summary>
        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.Append($"[{EventType.ToString()}]");

            if (Size.HasValue)
            {
                sb.Append($" Size: {Size?.ToString()}");
            }

            if (LogicalPoint.HasValue)
            {
                sb.Append($" Position: {LogicalPoint?.ToString()}");
            }

            return sb.ToString();
        }
    }
}