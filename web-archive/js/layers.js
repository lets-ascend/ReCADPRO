/**
 * Layer Management System
 */
class LayerManager {
    constructor(drawingEngine) {
        this.engine = drawingEngine;
        this.layers = [];
        this.activeLayerIndex = 0;
        this.layerCanvases = [];
        this.container = null;
        
        this.init();
    }
    
    init() {
        // Create initial layer
        this.addLayer('Layer 1');
    }
    
    addLayer(name = null) {
        const layerName = name || `Layer ${this.layers.length + 1}`;
        const layer = {
            id: Date.now(),
            name: layerName,
            visible: true,
            opacity: 1.0,
            locked: false
        };
        
        this.layers.push(layer);
        
        // Create canvas for this layer
        const canvas = document.createElement('canvas');
        canvas.width = this.engine.canvas.width;
        canvas.height = this.engine.canvas.height;
        canvas.style.position = 'absolute';
        canvas.style.top = this.engine.canvas.offsetTop + 'px';
        canvas.style.left = this.engine.canvas.offsetLeft + 'px';
        canvas.style.zIndex = this.layers.length;
        canvas.dataset.layerId = layer.id;
        
        const rect = this.engine.canvas.getBoundingClientRect();
        canvas.style.width = rect.width + 'px';
        canvas.style.height = rect.height + 'px';
        
        this.engine.canvas.parentElement.appendChild(canvas);
        this.layerCanvases.push(canvas);
        
        this.updateLayerUI();
        return layer;
    }
    
    removeLayer(index) {
        if (this.layers.length <= 1) {
            alert('Cannot remove the last layer');
            return;
        }
        
        const layer = this.layers[index];
        const canvas = this.layerCanvases[index];
        
        // Remove canvas
        if (canvas && canvas.parentElement) {
            canvas.parentElement.removeChild(canvas);
        }
        
        // Remove from arrays
        this.layers.splice(index, 1);
        this.layerCanvases.splice(index, 1);
        
        // Adjust active layer if needed
        if (this.activeLayerIndex >= this.layers.length) {
            this.activeLayerIndex = this.layers.length - 1;
        }
        
        this.updateLayerUI();
        this.renderAllLayers();
    }
    
    setActiveLayer(index) {
        if (index >= 0 && index < this.layers.length) {
            this.activeLayerIndex = index;
            this.updateLayerUI();
            this.switchToLayer(index);
        }
    }
    
    switchToLayer(index) {
        // Copy layer canvas content to main drawing canvas
        const layerCanvas = this.layerCanvases[index];
        if (layerCanvas) {
            this.engine.ctx.clearRect(0, 0, this.engine.canvas.width, this.engine.canvas.height);
            this.engine.ctx.drawImage(layerCanvas, 0, 0);
        }
    }
    
    getActiveLayerCanvas() {
        return this.layerCanvases[this.activeLayerIndex];
    }
    
    toggleLayerVisibility(index) {
        if (index >= 0 && index < this.layers.length) {
            this.layers[index].visible = !this.layers[index].visible;
            const canvas = this.layerCanvases[index];
            if (canvas) {
                canvas.style.display = this.layers[index].visible ? 'block' : 'none';
            }
            this.updateLayerUI();
        }
    }
    
    updateLayerUI() {
        const layersList = document.getElementById('layers-list');
        if (!layersList) return;
        
        layersList.innerHTML = '';
        
        this.layers.forEach((layer, index) => {
            const layerItem = document.createElement('div');
            layerItem.className = `layer-item ${index === this.activeLayerIndex ? 'active' : ''}`;
            layerItem.dataset.layerIndex = index;
            
            layerItem.innerHTML = `
                <span class="layer-name">${layer.name}</span>
                <div class="layer-controls">
                    <button class="layer-btn toggle-visibility" title="Toggle Visibility">
                        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor">
                            ${layer.visible 
                                ? '<path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path><circle cx="12" cy="12" r="3"></circle>'
                                : '<path d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"></path><line x1="1" y1="1" x2="23" y2="23"></line>'
                            }
                        </svg>
                    </button>
                    <button class="layer-btn delete-layer" title="Delete">
                        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor">
                            <polyline points="3 6 5 6 21 6"></polyline>
                            <path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path>
                        </svg>
                    </button>
                </div>
            `;
            
            // Add event listeners
            layerItem.querySelector('.toggle-visibility').addEventListener('click', (e) => {
                e.stopPropagation();
                this.toggleLayerVisibility(index);
            });
            
            layerItem.querySelector('.delete-layer').addEventListener('click', (e) => {
                e.stopPropagation();
                this.removeLayer(index);
            });
            
            layerItem.addEventListener('click', () => {
                this.setActiveLayer(index);
            });
            
            layersList.appendChild(layerItem);
        });
    }
    
    renderAllLayers() {
        // Clear main canvas
        this.engine.ctx.clearRect(0, 0, this.engine.canvas.width, this.engine.canvas.height);
        
        // Draw all visible layers
        this.layerCanvases.forEach((canvas, index) => {
            if (this.layers[index].visible) {
                const ctx = canvas.getContext('2d');
                ctx.globalAlpha = this.layers[index].opacity;
                this.engine.ctx.drawImage(canvas, 0, 0);
            }
        });
    }
    
    saveLayerToCanvas(layerIndex) {
        const layerCanvas = this.layerCanvases[layerIndex];
        if (layerCanvas) {
            const ctx = layerCanvas.getContext('2d');
            ctx.clearRect(0, 0, layerCanvas.width, layerCanvas.height);
            ctx.drawImage(this.engine.canvas, 0, 0);
        }
    }
    
    exportLayers() {
        return this.layers.map((layer, index) => {
            const canvas = this.layerCanvases[index];
            return {
                name: layer.name,
                visible: layer.visible,
                opacity: layer.opacity,
                data: canvas ? canvas.toDataURL() : null
            };
        });
    }
    
    importLayers(layersData) {
        this.layers = [];
        this.layerCanvases.forEach(canvas => {
            if (canvas.parentElement) {
                canvas.parentElement.removeChild(canvas);
            }
        });
        this.layerCanvases = [];
        
        layersData.forEach((layerData, index) => {
            const layer = this.addLayer(layerData.name);
            layer.visible = layerData.visible;
            layer.opacity = layerData.opacity;
            
            if (layerData.data && this.layerCanvases[index]) {
                const img = new Image();
                img.onload = () => {
                    const ctx = this.layerCanvases[index].getContext('2d');
                    ctx.drawImage(img, 0, 0);
                };
                img.src = layerData.data;
            }
        });
        
        this.setActiveLayer(0);
    }
}

