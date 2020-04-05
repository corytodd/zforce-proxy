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
        public TouchEvent(ZEventType eventType, Size? size, Point? position)
        {
            IsValid = true;
            EventType = eventType;
            Size = size;
            Position = position;
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
        public readonly Point? Position;

        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.Append($"[{EventType.ToString()}]");

            if (Size.HasValue)
            {
                sb.Append($" Size: {Size?.ToString()}");
            }

            if (Position.HasValue)
            {
                sb.Append($" Position: {Position?.ToString()}");
            }

            return sb.ToString();
        }
    }
}