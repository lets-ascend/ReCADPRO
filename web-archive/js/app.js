/**
 * Main Application Controller
 */
class App {
    constructor() {
        this.drawingEngine = null;
        this.drawingTools = null;
        this.layerManager = null;
        this.draftingTools = null;
        this.currentTool = 'pen';
        this.undoStack = [];
        this.redoStack = [];
        this.maxUndoSteps = 50;
        
        this.init();
    }
    
    init() {
        // Initialize drawing engine
        this.drawingEngine = new DrawingEngine('drawing-canvas');
        
        // Initialize tools
        this.drawingTools = new DrawingTools(this.drawingEngine);
        
        // Initialize layer manager
        this.layerManager = new LayerManager(this.drawingEngine);
        
        // Initialize drafting tools
        const gridCanvas = document.getElementById('grid-canvas');
        const rulerCanvas = document.getElementById('ruler-canvas');
        this.draftingTools = new DraftingTools(this.drawingEngine, gridCanvas, rulerCanvas);
        
        // Setup UI event listeners
        this.setupUIListeners();
        
        // Setup keyboard shortcuts
        this.setupKeyboardShortcuts();
        
        // Initial save state
        this.saveState();
        
        console.log('reMarkable Drafting Pro initialized');
    }
    
    setupUIListeners() {
        // Tool selection
        document.getElementById('tool-select').addEventListener('change', (e) => {
            this.setTool(e.target.value);
        });
        
        document.querySelectorAll('.tool-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const tool = e.currentTarget.dataset.tool;
                if (tool) {
                    this.setTool(tool);
                }
            });
        });
        
        // Line width
        const lineWidthSlider = document.getElementById('line-width');
        const lineWidthValue = document.getElementById('line-width-value');
        lineWidthSlider.addEventListener('input', (e) => {
            const value = e.target.value;
            lineWidthValue.textContent = value + 'px';
            this.drawingEngine.setLineWidth(parseInt(value));
            document.getElementById('size-slider').value = value;
            document.getElementById('size-value').textContent = value + 'px';
        });
        
        document.getElementById('size-slider').addEventListener('input', (e) => {
            const value = e.target.value;
            document.getElementById('size-value').textContent = value + 'px';
            this.drawingEngine.setLineWidth(parseInt(value));
            lineWidthSlider.value = value;
            lineWidthValue.textContent = value + 'px';
        });
        
        // Color picker
        document.getElementById('color-picker').addEventListener('change', (e) => {
            this.drawingEngine.setColor(e.target.value);
        });
        
        // Opacity
        document.getElementById('opacity').addEventListener('input', (e) => {
            const value = e.target.value;
            document.getElementById('opacity-value').textContent = value + '%';
            this.drawingEngine.setOpacity(parseInt(value));
        });
        
        // Pressure sensitivity
        document.getElementById('pressure-sensitive').addEventListener('change', (e) => {
            this.drawingEngine.setPressureSensitive(e.target.checked);
        });
        
        // Grid toggle
        document.getElementById('grid-toggle').addEventListener('click', () => {
            const enabled = this.draftingTools.toggleGrid();
            document.getElementById('grid-toggle').classList.toggle('active', enabled);
        });
        
        // Snap to grid
        document.getElementById('snap-toggle').addEventListener('click', () => {
            const enabled = this.draftingTools.toggleSnapToGrid();
            document.getElementById('snap-toggle').classList.toggle('active', enabled);
        });
        
        // Layers
        document.getElementById('add-layer-btn').addEventListener('click', () => {
            this.layerManager.addLayer();
            this.saveState();
        });
        
        // File menu
        document.getElementById('file-menu-btn').addEventListener('click', (e) => {
            e.stopPropagation();
            const menu = document.getElementById('file-menu');
            menu.classList.toggle('show');
        });
        
        document.addEventListener('click', (e) => {
            if (!e.target.closest('#file-menu-btn') && !e.target.closest('#file-menu')) {
                document.getElementById('file-menu').classList.remove('show');
            }
        });
        
        // File operations
        document.getElementById('new-file').addEventListener('click', () => {
            if (confirm('Create a new drawing? Unsaved changes will be lost.')) {
                this.newFile();
            }
        });
        
        document.getElementById('open-file').addEventListener('click', () => {
            this.openFile();
        });
        
        document.getElementById('save-file').addEventListener('click', () => {
            this.saveFile();
        });
        
        // Undo/Redo
        document.getElementById('undo-btn').addEventListener('click', () => {
            this.undo();
        });
        
        document.getElementById('redo-btn').addEventListener('click', () => {
            this.redo();
        });
        
        // Templates
        document.querySelectorAll('.template-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const template = e.currentTarget.dataset.template;
                this.applyTemplate(template);
            });
        });
        
        // Export
        document.getElementById('export-png').addEventListener('click', () => {
            this.exportPNG();
        });
        
        document.getElementById('export-pdf').addEventListener('click', () => {
            this.exportPDF();
        });
        
        document.getElementById('export-svg').addEventListener('click', () => {
            this.exportSVG();
        });
        
        document.getElementById('export-json').addEventListener('click', () => {
            this.exportJSON();
        });
        
        // Coordinates display
        this.drawingEngine.canvas.addEventListener('mousemove', (e) => {
            const coords = this.drawingEngine.getCoordinates(e);
            document.getElementById('coordinates').textContent = 
                `X: ${Math.round(coords.x)}, Y: ${Math.round(coords.y)}`;
        });
        
        // Save state on drawing
        let saveTimeout;
        this.drawingEngine.canvas.addEventListener('mouseup', () => {
            clearTimeout(saveTimeout);
            saveTimeout = setTimeout(() => {
                this.saveState();
            }, 500);
        });
    }
    
    setupKeyboardShortcuts() {
        document.addEventListener('keydown', (e) => {
            // Ctrl/Cmd + Z for undo
            if ((e.ctrlKey || e.metaKey) && e.key === 'z' && !e.shiftKey) {
                e.preventDefault();
                this.undo();
            }
            
            // Ctrl/Cmd + Shift + Z for redo
            if ((e.ctrlKey || e.metaKey) && e.key === 'z' && e.shiftKey) {
                e.preventDefault();
                this.redo();
            }
            
            // Ctrl/Cmd + S for save
            if ((e.ctrlKey || e.metaKey) && e.key === 's') {
                e.preventDefault();
                this.saveFile();
            }
            
            // Ctrl/Cmd + N for new
            if ((e.ctrlKey || e.metaKey) && e.key === 'n') {
                e.preventDefault();
                if (confirm('Create a new drawing? Unsaved changes will be lost.')) {
                    this.newFile();
                }
            }
            
            // G for grid toggle
            if (e.key === 'g' && !e.ctrlKey && !e.metaKey) {
                const enabled = this.draftingTools.toggleGrid();
                document.getElementById('grid-toggle').classList.toggle('active', enabled);
            }
        });
    }
    
    setTool(tool) {
        this.currentTool = tool;
        this.drawingTools.setTool(tool);
        document.getElementById('tool-select').value = tool;
        
        // Update active tool button
        document.querySelectorAll('.tool-btn').forEach(btn => {
            btn.classList.toggle('active', btn.dataset.tool === tool);
        });
        
        // Handle special tools
        if (tool === 'ruler') {
            this.draftingTools.rulerEnabled = true;
        } else {
            this.draftingTools.clearRuler();
        }
    }
    
    saveState() {
        const state = this.drawingEngine.canvas.toDataURL();
        this.undoStack.push(state);
        
        if (this.undoStack.length > this.maxUndoSteps) {
            this.undoStack.shift();
        }
        
        this.redoStack = [];
    }
    
    undo() {
        if (this.undoStack.length <= 1) return;
        
        const currentState = this.undoStack.pop();
        this.redoStack.push(currentState);
        
        const previousState = this.undoStack[this.undoStack.length - 1];
        this.drawingEngine.setImageData(previousState);
    }
    
    redo() {
        if (this.redoStack.length === 0) return;
        
        const state = this.redoStack.pop();
        this.undoStack.push(state);
        this.drawingEngine.setImageData(state);
    }
    
    newFile() {
        this.drawingEngine.clear();
        this.layerManager = new LayerManager(this.drawingEngine);
        this.undoStack = [];
        this.redoStack = [];
        this.saveState();
    }
    
    openFile() {
        const input = document.createElement('input');
        input.type = 'file';
        input.accept = '.json,.png';
        input.onchange = (e) => {
            const file = e.target.files[0];
            if (!file) return;
            
            const reader = new FileReader();
            reader.onload = (event) => {
                if (file.name.endsWith('.json')) {
                    const data = JSON.parse(event.target.result);
                    if (data.layers) {
                        this.layerManager.importLayers(data.layers);
                    }
                    if (data.drawing) {
                        this.drawingEngine.setImageData(data.drawing);
                    }
                } else {
                    this.drawingEngine.setImageData(event.target.result);
                }
                this.saveState();
            };
            reader.readAsDataURL(file);
        };
        input.click();
    }
    
    saveFile() {
        const data = {
            version: '1.0',
            timestamp: new Date().toISOString(),
            drawing: this.drawingEngine.getImageData(),
            layers: this.layerManager.exportLayers(),
            settings: {
                tool: this.currentTool,
                lineWidth: this.drawingEngine.lineWidth,
                color: this.drawingEngine.color,
                opacity: this.drawingEngine.opacity
            }
        };
        
        const blob = new Blob([JSON.stringify(data, null, 2)], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `drawing-${Date.now()}.json`;
        a.click();
        URL.revokeObjectURL(url);
    }
    
    applyTemplate(template) {
        switch (template) {
            case 'grid':
                this.draftingTools.setGridType('lines');
                this.draftingTools.toggleGrid();
                document.getElementById('grid-toggle').classList.add('active');
                break;
            case 'ruled':
                this.draftingTools.setGridType('lines');
                this.draftingTools.setGridSize(20);
                this.draftingTools.toggleGrid();
                document.getElementById('grid-toggle').classList.add('active');
                break;
            case 'isometric':
                this.draftingTools.setGridType('isometric');
                this.draftingTools.toggleGrid();
                document.getElementById('grid-toggle').classList.add('active');
                break;
            case 'dot':
                this.draftingTools.setGridType('dots');
                this.draftingTools.toggleGrid();
                document.getElementById('grid-toggle').classList.add('active');
                break;
        }
    }
    
    exportPNG() {
        const dataUrl = this.drawingEngine.getImageData();
        const a = document.createElement('a');
        a.href = dataUrl;
        a.download = `drawing-${Date.now()}.png`;
        a.click();
    }
    
    exportPDF() {
        // Simple PDF export using canvas
        const imgData = this.drawingEngine.getImageData();
        const img = new Image();
        img.onload = () => {
            // Note: For full PDF support, you'd want to use a library like jsPDF
            alert('PDF export requires jsPDF library. PNG export available.');
            this.exportPNG();
        };
        img.src = imgData;
    }
    
    exportSVG() {
        const svg = this.drawingEngine.exportSVG();
        const blob = new Blob([svg], { type: 'image/svg+xml' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `drawing-${Date.now()}.svg`;
        a.click();
        URL.revokeObjectURL(url);
    }
    
    exportJSON() {
        this.saveFile();
    }
}

// Initialize app when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    window.app = new App();
});

