/**
 * Advanced Drafting Tools - Rulers, Grids, Snap-to-Grid, Measurements
 */
class DraftingTools {
    constructor(drawingEngine, gridCanvas, rulerCanvas) {
        this.engine = drawingEngine;
        this.gridCanvas = gridCanvas;
        this.rulerCanvas = rulerCanvas;
        this.gridCtx = this.gridCanvas.getContext('2d');
        this.rulerCtx = this.rulerCanvas.getContext('2d');
        
        this.gridEnabled = false;
        this.snapToGrid = false;
        this.gridSize = 20;
        this.gridColor = '#e0e0e0';
        this.gridType = 'lines'; // 'lines', 'dots', 'isometric'
        
        this.rulerEnabled = false;
        this.rulerStartX = 0;
        this.rulerStartY = 0;
        this.rulerEndX = 0;
        this.rulerEndY = 0;
        this.isRulerActive = false;
        
        this.setupCanvases();
    }
    
    setupCanvases() {
        // Match grid and ruler canvas sizes to main canvas
        const updateCanvasSize = () => {
            const rect = this.engine.canvas.getBoundingClientRect();
            const dpr = window.devicePixelRatio || 1;
            
            [this.gridCanvas, this.rulerCanvas].forEach(canvas => {
                canvas.width = this.engine.canvas.width;
                canvas.height = this.engine.canvas.height;
                canvas.style.width = rect.width + 'px';
                canvas.style.height = rect.height + 'px';
            });
            
            this.gridCtx.scale(dpr, dpr);
            this.rulerCtx.scale(dpr, dpr);
            
            if (this.gridEnabled) {
                this.drawGrid();
            }
        };
        
        updateCanvasSize();
        window.addEventListener('resize', updateCanvasSize);
    }
    
    toggleGrid() {
        this.gridEnabled = !this.gridEnabled;
        if (this.gridEnabled) {
            this.drawGrid();
        } else {
            this.gridCtx.clearRect(0, 0, this.gridCanvas.width, this.gridCanvas.height);
        }
        return this.gridEnabled;
    }
    
    drawGrid() {
        this.gridCtx.clearRect(0, 0, this.gridCanvas.width, this.gridCanvas.height);
        
        if (!this.gridEnabled) return;
        
        const rect = this.engine.canvas.getBoundingClientRect();
        const width = rect.width;
        const height = rect.height;
        
        this.gridCtx.strokeStyle = this.gridColor;
        this.gridCtx.lineWidth = 1;
        
        switch (this.gridType) {
            case 'lines':
                this.drawLineGrid(width, height);
                break;
            case 'dots':
                this.drawDotGrid(width, height);
                break;
            case 'isometric':
                this.drawIsometricGrid(width, height);
                break;
        }
    }
    
    drawLineGrid(width, height) {
        // Vertical lines
        for (let x = 0; x <= width; x += this.gridSize) {
            this.gridCtx.beginPath();
            this.gridCtx.moveTo(x, 0);
            this.gridCtx.lineTo(x, height);
            this.gridCtx.stroke();
        }
        
        // Horizontal lines
        for (let y = 0; y <= height; y += this.gridSize) {
            this.gridCtx.beginPath();
            this.gridCtx.moveTo(0, y);
            this.gridCtx.lineTo(width, y);
            this.gridCtx.stroke();
        }
    }
    
    drawDotGrid(width, height) {
        this.gridCtx.fillStyle = this.gridColor;
        for (let x = 0; x <= width; x += this.gridSize) {
            for (let y = 0; y <= height; y += this.gridSize) {
                this.gridCtx.beginPath();
                this.gridCtx.arc(x, y, 1, 0, Math.PI * 2);
                this.gridCtx.fill();
            }
        }
    }
    
    drawIsometricGrid(width, height) {
        const angle = Math.PI / 6; // 30 degrees
        
        // Diagonal lines (top-left to bottom-right)
        for (let i = -height; i <= width; i += this.gridSize) {
            this.gridCtx.beginPath();
            this.gridCtx.moveTo(i, 0);
            this.gridCtx.lineTo(i + height * Math.tan(angle), height);
            this.gridCtx.stroke();
        }
        
        // Diagonal lines (top-right to bottom-left)
        for (let i = 0; i <= width + height; i += this.gridSize) {
            this.gridCtx.beginPath();
            this.gridCtx.moveTo(i, 0);
            this.gridCtx.lineTo(i - height * Math.tan(angle), height);
            this.gridCtx.stroke();
        }
        
        // Horizontal lines
        for (let y = 0; y <= height; y += this.gridSize) {
            this.gridCtx.beginPath();
            this.gridCtx.moveTo(0, y);
            this.gridCtx.lineTo(width, y);
            this.gridCtx.stroke();
        }
    }
    
    setGridType(type) {
        this.gridType = type;
        if (this.gridEnabled) {
            this.drawGrid();
        }
    }
    
    setGridSize(size) {
        this.gridSize = size;
        if (this.gridEnabled) {
            this.drawGrid();
        }
    }
    
    snapPoint(x, y) {
        if (!this.snapToGrid) {
            return { x, y };
        }
        
        const snappedX = Math.round(x / this.gridSize) * this.gridSize;
        const snappedY = Math.round(y / this.gridSize) * this.gridSize;
        
        return { x: snappedX, y: snappedY };
    }
    
    toggleSnapToGrid() {
        this.snapToGrid = !this.snapToGrid;
        return this.snapToGrid;
    }
    
    // Ruler tool
    startRuler(x, y) {
        this.rulerEnabled = true;
        this.isRulerActive = true;
        this.rulerStartX = x;
        this.rulerStartY = y;
        this.rulerEndX = x;
        this.rulerEndY = y;
        this.drawRuler();
    }
    
    updateRuler(x, y) {
        if (!this.isRulerActive) return;
        this.rulerEndX = x;
        this.rulerEndY = y;
        this.drawRuler();
    }
    
    finishRuler() {
        this.isRulerActive = false;
    }
    
    drawRuler() {
        this.rulerCtx.clearRect(0, 0, this.rulerCanvas.width, this.rulerCanvas.height);
        
        if (!this.rulerEnabled) return;
        
        const dx = this.rulerEndX - this.rulerStartX;
        const dy = this.rulerEndY - this.rulerStartY;
        const distance = Math.sqrt(dx * dx + dy * dy);
        
        // Draw ruler line
        this.rulerCtx.strokeStyle = '#0066cc';
        this.rulerCtx.lineWidth = 2;
        this.rulerCtx.setLineDash([5, 5]);
        this.rulerCtx.beginPath();
        this.rulerCtx.moveTo(this.rulerStartX, this.rulerStartY);
        this.rulerCtx.lineTo(this.rulerEndX, this.rulerEndY);
        this.rulerCtx.stroke();
        this.rulerCtx.setLineDash([]);
        
        // Draw measurement text
        const midX = (this.rulerStartX + this.rulerEndX) / 2;
        const midY = (this.rulerStartY + this.rulerEndY) / 2;
        
        this.rulerCtx.fillStyle = '#0066cc';
        this.rulerCtx.font = '14px sans-serif';
        this.rulerCtx.textAlign = 'center';
        this.rulerCtx.fillText(`${distance.toFixed(1)}px`, midX, midY - 10);
        
        // Draw start and end markers
        this.drawRulerMarker(this.rulerStartX, this.rulerStartY);
        this.drawRulerMarker(this.rulerEndX, this.rulerEndY);
    }
    
    drawRulerMarker(x, y) {
        this.rulerCtx.fillStyle = '#0066cc';
        this.rulerCtx.beginPath();
        this.rulerCtx.arc(x, y, 4, 0, Math.PI * 2);
        this.rulerCtx.fill();
    }
    
    clearRuler() {
        this.rulerEnabled = false;
        this.isRulerActive = false;
        this.rulerCtx.clearRect(0, 0, this.rulerCanvas.width, this.rulerCanvas.height);
    }
    
    // Protractor tool
    drawProtractor(centerX, centerY, radius, angle) {
        this.rulerCtx.clearRect(0, 0, this.rulerCanvas.width, this.rulerCanvas.height);
        
        // Draw arc
        this.rulerCtx.strokeStyle = '#0066cc';
        this.rulerCtx.lineWidth = 2;
        this.rulerCtx.beginPath();
        this.rulerCtx.arc(centerX, centerY, radius, 0, angle);
        this.rulerCtx.stroke();
        
        // Draw angle text
        const angleDegrees = (angle * 180 / Math.PI).toFixed(1);
        this.rulerCtx.fillStyle = '#0066cc';
        this.rulerCtx.font = '14px sans-serif';
        this.rulerCtx.textAlign = 'center';
        this.rulerCtx.fillText(`${angleDegrees}°`, centerX, centerY - radius - 10);
    }
}

