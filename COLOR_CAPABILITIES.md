# reMarkable Paper Pro Color Capabilities

## Display Technology

The reMarkable Paper Pro features an **11.8-inch Canvas Color display** capable of rendering **up to 20,000 colors**.

### Color Characteristics

1. **Color Range**: Up to 20,000 colors
   - Achieved through primary colors and dithering techniques
   - More muted/pastel compared to LCD/OLED displays
   - Paper-like color appearance

2. **Color Palette**: 
   - 9 base colors available (including magenta and cyan)
   - Colors can be blended and layered
   - Creates additional hues through mixing

3. **Color Saturation**:
   - Colors are subdued/muted
   - Resemble pastel tones
   - Optimized for paper-like experience
   - Not as vibrant as traditional screens

## Implementation Considerations

### Color Support
- ✅ **Full color support** - Not grayscale limited
- ✅ **RGB color model** - Standard RGB colors work
- ✅ **Color blending** - Can blend colors through layering
- ✅ **Dithering** - Display uses dithering for color range

### Optimization for Canvas Color Display
- Colors will appear more muted than on LCD/OLED
- Pastel-like appearance is normal
- High contrast colors work best
- Avoid very subtle color differences

### Color Palette Recommendations
- Use distinct colors for better visibility
- High contrast combinations recommended
- Pastel/muted colors are natural on this display
- Test color combinations for visibility

## Application Implementation

### Color Selection
- Support full RGB color picker
- Provide preset color palettes optimized for display
- Allow color blending/mixing
- Show color preview accounting for muted display

### Import Handling
- **DO NOT** convert to grayscale automatically
- Preserve original colors
- May need slight saturation adjustment for visibility
- Test imported images for best appearance

### Drawing Tools
- All tools support full color
- Pressure sensitivity can affect color intensity
- Layer blending modes for color mixing
- Color picker with display-optimized presets

## Differences from Standard Displays

| Aspect | LCD/OLED | Canvas Color Display |
|--------|----------|---------------------|
| Color Vibrancy | High | Muted/Pastel |
| Color Range | Millions | ~20,000 |
| Appearance | Bright | Paper-like |
| Best Use | Vibrant colors | Subtle, natural colors |

## Best Practices

1. **Color Selection**: Choose distinct, high-contrast colors
2. **Testing**: Test color combinations on actual device
3. **Import**: Preserve original colors, don't force grayscale
4. **Blending**: Use layering for color mixing
5. **Contrast**: Ensure sufficient contrast for visibility

## Code Updates Needed

- ❌ Remove automatic grayscale conversion
- ✅ Support full RGB color
- ✅ Add color palette presets
- ✅ Implement color blending
- ✅ Optimize for muted display appearance

