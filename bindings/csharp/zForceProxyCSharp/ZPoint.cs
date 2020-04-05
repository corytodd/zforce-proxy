namespace zForceProxyCSharp
{
    /// <summary>
    ///    Logical point for abstracting the device resolution 
    /// </summary>
    public readonly struct ZPoint
    {
        /// <summary>
        /// Creates a new Zpoint which is a logical coordinate in range [0,1]
        /// </summary>
        /// <param name="x">logical x axis position</param>
        /// <param name="y">logical y axis position</param>
        /// <param name="z">logical z axis position</param>
        public ZPoint(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        ///     Logical X coordinate
        /// </summary>
        public readonly float X;

        /// <summary>
        ///     Logical X coordinate
        /// </summary>
        public readonly float Y;

        /// <summary>
        ///     Logical X coordinate
        /// </summary>
        public readonly float Z;

        /// <summary>
        ///     Returns (X,Y,Z)
        /// </summary>
        public override string ToString()
        {
            return $"({X},{Y},{Z})";
        }
    }
}