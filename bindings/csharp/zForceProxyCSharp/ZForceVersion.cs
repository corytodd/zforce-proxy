namespace zForceProxyCSharp
{
    /// <summary>
    ///     Library version in semver components
    /// </summary>
    public readonly struct ZForceVersion
    {
        /// <summary>
        ///     Create a new version from a packed integer
        /// </summary>
        /// <param name="version">semver integer</param>
        public ZForceVersion(int version)
        {
            _major = (version >> 24) & 0xFF;
            _minor = (version >> 16) & 0xFF;
            _patch = (version >> 8) & 0xFF;
            _tweak = version & 0xFF;
        }

        /// <summary>
        ///     Major component of version
        /// </summary>
        private readonly int _major;

        /// <summary>
        ///     Minor component of version
        /// </summary>
        private readonly int _minor;

        /// <summary>
        ///     Patch component of version
        /// </summary>
        private readonly int _patch;

        /// <summary>
        ///     Tweak component of version
        /// </summary>
        private readonly int _tweak;

        /// <summary>
        ///     Return Major.Minor.Patch.Tweak
        /// </summary>
        public override string ToString()
        {
            return $"{_major}.{_minor}.{_patch}.{_tweak}";
        }
    }
}