/**
 * Computer Graphics Hub — Application Logic & Controller
 * Vanilla JS, reactive filtering, hash routing, modal PDF viewer.
 */

(function () {
  'use strict';

  // Application State
  const state = {
    materials: [],
    labs: [],
    projects: [],
    filteredMaterials: [],
    activeTab: 'overview',
    activeCategory: 'Tutti',
    searchQuery: '',
    theme: localStorage.getItem('cg_theme') || 'dark',
    currentPdf: null
  };

  // DOM Elements
  const el = {
    themeToggleBtn: document.getElementById('themeToggleBtn'),
    searchInput: document.getElementById('globalSearchInput'),
    searchClearBtn: document.getElementById('searchClearBtn'),
    mobileMenuBtn: document.getElementById('mobileMenuBtn'),
    sidebar: document.querySelector('.app-sidebar'),
    navItems: document.querySelectorAll('.nav-item'),
    tabPanes: document.querySelectorAll('.tab-pane'),
    materialsContainer: document.getElementById('materialsGrid'),
    materialsCountEl: document.getElementById('materialsCount'),
    filterPillsContainer: document.getElementById('filterPills'),
    desktopLabsGrid: document.getElementById('desktopLabsGrid'),
    webLabsGrid: document.getElementById('webLabsGrid'),
    projectsGrid: document.getElementById('projectsGrid'),
    statPdfCount: document.getElementById('statPdfCount'),
    statLabCount: document.getElementById('statLabCount'),
    statWebCount: document.getElementById('statWebCount'),
    statProjectCount: document.getElementById('statProjectCount'),
    // Modal
    pdfModal: document.getElementById('pdfModal'),
    modalTitle: document.getElementById('modalTitle'),
    modalIframe: document.getElementById('modalIframe'),
    modalCloseBtn: document.getElementById('modalCloseBtn'),
    modalDownloadBtn: document.getElementById('modalDownloadBtn'),
    modalExternalBtn: document.getElementById('modalExternalBtn')
  };

  // Initialize
  async function init() {
    applyTheme(state.theme);
    setupEvents();
    await loadData();
    handleHashChange();
  }

  // Load JSON Data
  async function loadData() {
    try {
      const [matRes, labRes, projRes] = await Promise.all([
        fetch('data/materials.json'),
        fetch('data/labs.json'),
        fetch('data/projects.json')
      ]);

      state.materials = await matRes.json();
      state.labs = await labRes.json();
      state.projects = await projRes.json();

      updateStats();
      buildFilterPills();
      renderMaterials();
      renderDesktopLabs();
      renderWebLabs();
      renderProjects();
    } catch (err) {
      console.error('Errore nel caricamento dei dati del portale:', err);
    }
  }

  // Stats
  function updateStats() {
    if (el.statPdfCount) el.statPdfCount.textContent = state.materials.length;
    if (el.statLabCount) el.statLabCount.textContent = state.labs.length;
    if (el.statWebCount) {
      const webReady = state.labs.filter(l => l.webStatusBadge === 'available').length;
      el.statWebCount.textContent = `${webReady} Pronti`;
    }
    if (el.statProjectCount) el.statProjectCount.textContent = state.projects.length;
  }

  // Category Filter Pills
  function buildFilterPills() {
    if (!el.filterPillsContainer) return;
    const categories = ['Tutti'];
    const catCounts = { 'Tutti': state.materials.length };

    state.materials.forEach(m => {
      catCounts[m.category] = (catCounts[m.category] || 0) + 1;
      if (!categories.includes(m.category)) {
        categories.push(m.category);
      }
    });

    el.filterPillsContainer.innerHTML = categories.map(cat => `
      <button class="filter-pill ${cat === state.activeCategory ? 'active' : ''}" data-cat="${escapeHtml(cat)}">
        ${escapeHtml(cat)} (${catCounts[cat]})
      </button>
    `).join('');

    el.filterPillsContainer.querySelectorAll('.filter-pill').forEach(btn => {
      btn.addEventListener('click', () => {
        state.activeCategory = btn.dataset.cat;
        el.filterPillsContainer.querySelectorAll('.filter-pill').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        renderMaterials();
      });
    });
  }

  // Render Materials Grid
  function renderMaterials() {
    if (!el.materialsContainer) return;

    let items = state.materials;

    // Filter by category
    if (state.activeCategory !== 'Tutti') {
      items = items.filter(m => m.category === state.activeCategory);
    }

    // Filter by search query
    if (state.searchQuery.trim() !== '') {
      const q = state.searchQuery.toLowerCase();
      items = items.filter(m =>
        m.title.toLowerCase().includes(q) ||
        m.category.toLowerCase().includes(q) ||
        (m.tags && m.tags.some(t => t.toLowerCase().includes(q))) ||
        (m.description && m.description.toLowerCase().includes(q))
      );
    }

    state.filteredMaterials = items;
    if (el.materialsCountEl) {
      el.materialsCountEl.textContent = `${items.length} ${items.length === 1 ? 'documento trovato' : 'documenti trovati'}`;
    }

    if (items.length === 0) {
      el.materialsContainer.innerHTML = `
        <div style="grid-column: 1 / -1; text-align: center; padding: 48px; color: var(--text-muted);">
          <svg style="width:48px; height:48px; margin-bottom:12px; stroke: currentColor; fill:none;" viewBox="0 0 24 24">
            <circle cx="11" cy="11" r="8"></circle>
            <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
          </svg>
          <p style="font-size: 1.1rem; font-weight:600; color:var(--text-secondary);">Nessun documento corrisponde ai criteri</p>
          <p style="font-size: 0.85rem;">Prova a reimpostare la ricerca o seleziona un'altra categoria.</p>
        </div>
      `;
      return;
    }

    el.materialsContainer.innerHTML = items.map(m => {
      const fileUrl = `../${encodeURI(m.relPath)}`;
      return `
        <div class="material-card">
          <div>
            <div class="card-top">
              <div class="pdf-badge-icon">PDF</div>
              <div class="card-title-group">
                <span class="card-category-tag">${escapeHtml(m.category)}</span>
                <h3>${escapeHtml(m.title)}</h3>
              </div>
            </div>
            <p class="card-desc">${escapeHtml(m.description || '')}</p>
            <div class="card-tags">
              ${(m.tags || []).map(t => `<span class="tag-badge">${escapeHtml(t)}</span>`).join('')}
              <span class="tag-badge" style="color:var(--accent-purple);">${escapeHtml(m.academicYear)}</span>
            </div>
          </div>
          <div class="card-footer">
            <span class="file-size-info">${escapeHtml(m.size)}</span>
            <div class="card-actions">
              <button class="action-btn view" onclick="window.CGHub.openPdfViewer('${escapeAttr(m.title)}', '${escapeAttr(fileUrl)}')">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/><circle cx="12" cy="12" r="3"/></svg>
                Leggi
              </button>
              <a href="${fileUrl}" download="${escapeAttr(m.filename)}" class="action-btn download" title="Scarica PDF">
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/><polyline points="7 10 12 15 17 10"/><line x1="12" y1="15" x2="12" y2="3"/></svg>
              </a>
            </div>
          </div>
        </div>
      `;
    }).join('');
  }

  // Render Desktop Labs
  function renderDesktopLabs() {
    if (!el.desktopLabsGrid) return;

    el.desktopLabsGrid.innerHTML = state.labs.map(lab => {
      const docLink = lab.docPdf ? `../${encodeURI(lab.docPdf)}` : null;
      return `
        <div class="lab-card">
          <span class="lab-number-badge">LAB ${escapeHtml(lab.number)}</span>
          <div>
            <div class="lab-header">
              <span class="lab-type-tag">${escapeHtml(lab.type)} • ${escapeHtml(lab.date)}</span>
              <h3 class="lab-title">${escapeHtml(lab.title)}</h3>
              <p class="lab-subtitle">${escapeHtml(lab.subtitle)}</p>
            </div>
            <p class="lab-desc">${escapeHtml(lab.description)}</p>

            <div class="lab-meta-box">
              <div class="lab-meta-row">
                <span class="lab-meta-label">Cartella Originale:</span>
                <span class="lab-meta-val" style="font-size:0.75rem;">${escapeHtml(lab.folder)}</span>
              </div>
              <div class="lab-meta-row">
                <span class="lab-meta-label">Stato Web:</span>
                <span class="badge-status ${escapeHtml(lab.webStatusBadge)}">${escapeHtml(lab.webStatus)}</span>
              </div>
            </div>

            <div style="margin-bottom: 16px;">
              <span style="font-size:0.75rem; font-weight:700; color:var(--text-muted); text-transform:uppercase;">Obiettivi Didattici:</span>
              <ul style="list-style:none; margin-top:6px;">
                ${lab.objectives.map(obj => `
                  <li style="font-size:0.8rem; color:var(--text-secondary); margin-bottom:4px; display:flex; gap:6px;">
                    <span style="color:var(--accent-cyan); font-weight:bold;">▸</span> ${escapeHtml(obj)}
                  </li>
                `).join('')}
              </ul>
            </div>

            <div class="card-tags" style="margin-bottom: 16px;">
              ${lab.techStack.map(t => `<span class="tag-badge" style="color:var(--accent-blue);">${escapeHtml(t)}</span>`).join('')}
            </div>
          </div>

          <div class="lab-card-actions">
            ${docLink ? `
              <button class="topbar-btn" onclick="window.CGHub.openPdfViewer('${escapeAttr(lab.title)}', '${escapeAttr(docLink)}')">
                <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/></svg>
                Dispensa
              </button>
            ` : ''}
            ${lab.webUrl ? `
              <a href="${escapeAttr(lab.webUrl)}" class="topbar-btn primary">
                <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="5 3 19 12 5 21 5 3"/></svg>
                Versione Web
              </a>
            ` : ''}
          </div>
        </div>
      `;
    }).join('');
  }

  // Render Web Labs
  function renderWebLabs() {
    if (!el.webLabsGrid) return;

    const webLabs = state.labs.filter(l => l.webUrl);

    el.webLabsGrid.innerHTML = webLabs.map(lab => `
      <div class="lab-card" style="border-top: 3px solid var(--accent-cyan);">
        <span class="lab-number-badge">WEB LAB ${escapeHtml(lab.number)}</span>
        <div>
          <div class="lab-header">
            <span class="lab-type-tag" style="color:var(--accent-emerald);">Interattivo nel Browser • ${escapeHtml(lab.type)}</span>
            <h3 class="lab-title">${escapeHtml(lab.title)}</h3>
            <p class="lab-subtitle">${escapeHtml(lab.subtitle)}</p>
          </div>
          <p class="lab-desc">${escapeHtml(lab.description)}</p>

          <div class="card-tags" style="margin-bottom: 20px;">
            ${lab.techStack.map(t => `<span class="tag-badge">${escapeHtml(t)}</span>`).join('')}
          </div>
        </div>

        <div>
          <a href="${escapeAttr(lab.webUrl)}" class="topbar-btn primary" style="width: 100%; justify-content: center; padding: 12px;">
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="5 3 19 12 5 21 5 3"/></svg>
            Esegui Laboratorio nel Browser
          </a>
        </div>
      </div>
    `).join('');
  }

  // Render Projects Grid
  function renderProjects() {
    if (!el.projectsGrid) return;

    el.projectsGrid.innerHTML = state.projects.map(p => `
      <div class="project-card">
        <div>
          <div class="project-header">
            <div>
              <span class="project-category">${escapeHtml(p.category)}</span>
              <h3 class="project-title">${escapeHtml(p.name)}</h3>
            </div>
            <span class="badge-status ${escapeHtml(p.statusBadge)}">${escapeHtml(p.status)}</span>
          </div>
          <p class="project-desc">${escapeHtml(p.description)}</p>

          <ul class="project-highlights">
            ${(p.highlights || []).map(h => `<li>${escapeHtml(h)}</li>`).join('')}
          </ul>

          <div class="card-tags" style="margin-bottom: 20px;">
            ${p.tech.map(t => `<span class="tag-badge" style="color:var(--accent-cyan);">${escapeHtml(t)}</span>`).join('')}
          </div>
        </div>

        <div style="display:flex; gap:10px;">
          ${p.githubUrl ? `
            <a href="${escapeAttr(p.githubUrl)}" target="_blank" rel="noopener" class="topbar-btn" style="flex:1; justify-content:center;">
              <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 19c-5 1.5-5-2.5-7-3m14 6v-3.87a3.37 3.37 0 0 0-.94-2.61c3.14-.35 6.44-1.54 6.44-7A5.44 5.44 0 0 0 20 4.77 5.07 5.07 0 0 0 19.91 1S18.73.65 16 2.48a13.38 13.38 0 0 0-7 0C6.27.65 5.09 1 5.09 1A5.07 5.07 0 0 0 5 4.77a5.44 5.44 0 0 0-1.5 3.78c0 5.42 3.3 6.61 6.44 7A3.37 3.37 0 0 0 9 18.13V22"/></svg>
              GitHub Repo
            </a>
          ` : ''}
          ${p.demoUrl ? `
            <a href="${escapeAttr(p.demoUrl)}" target="_blank" rel="noopener" class="topbar-btn primary" style="flex:1; justify-content:center;">
              <svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polygon points="10 8 16 12 10 16 10 8"/></svg>
              Live Demo
            </a>
          ` : ''}
        </div>
      </div>
    `).join('');
  }

  // PDF Viewer Modal
  function openPdfViewer(title, url) {
    state.currentPdf = { title, url };
    if (el.modalTitle) el.modalTitle.textContent = title;
    if (el.modalIframe) el.modalIframe.src = url;
    if (el.modalDownloadBtn) el.modalDownloadBtn.href = url;
    if (el.modalExternalBtn) el.modalExternalBtn.href = url;
    if (el.pdfModal) el.pdfModal.classList.add('active');
    document.body.style.overflow = 'hidden';
  }

  function closePdfViewer() {
    if (el.pdfModal) el.pdfModal.classList.remove('active');
    if (el.modalIframe) el.modalIframe.src = 'about:blank';
    document.body.style.overflow = '';
    state.currentPdf = null;
  }

  // Routing
  function switchTab(tabId) {
    state.activeTab = tabId;
    window.location.hash = tabId;

    el.tabPanes.forEach(pane => {
      pane.classList.toggle('active', pane.id === `tab-${tabId}`);
    });

    el.navItems.forEach(item => {
      item.classList.toggle('active', item.dataset.tab === tabId);
    });

    if (el.sidebar) el.sidebar.classList.remove('open');
    window.scrollTo({ top: 0, behavior: 'smooth' });
  }

  function handleHashChange() {
    const hash = window.location.hash.replace('#', '');
    const validTabs = ['overview', 'materials', 'desktop-labs', 'web-labs', 'playground', 'projects', 'about'];
    if (validTabs.includes(hash)) {
      switchTab(hash);
    } else {
      switchTab('overview');
    }
  }

  // Theme Management
  function applyTheme(theme) {
    state.theme = theme;
    document.documentElement.setAttribute('data-theme', theme);
    localStorage.setItem('cg_theme', theme);
    if (el.themeToggleBtn) {
      el.themeToggleBtn.innerHTML = theme === 'light'
        ? `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"/></svg>`
        : `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="5"/><line x1="12" y1="1" x2="12" y2="3"/><line x1="12" y1="21" x2="12" y2="23"/><line x1="4.22" y1="4.22" x2="5.64" y2="5.64"/><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"/><line x1="1" y1="12" x2="3" y2="12"/><line x1="21" y1="12" x2="23" y2="12"/><line x1="4.22" y1="19.78" x2="5.64" y2="18.36"/><line x1="18.36" y1="5.64" x2="19.78" y2="4.22"/></svg>`;
    }
  }

  function toggleTheme() {
    applyTheme(state.theme === 'dark' ? 'light' : 'dark');
  }

  // Event Listeners
  function setupEvents() {
    // Navigation items
    el.navItems.forEach(item => {
      item.addEventListener('click', (e) => {
        e.preventDefault();
        const tab = item.dataset.tab;
        if (tab) switchTab(tab);
      });
    });

    window.addEventListener('hashchange', handleHashChange);

    // Theme toggle
    if (el.themeToggleBtn) el.themeToggleBtn.addEventListener('click', toggleTheme);

    // Search input
    if (el.searchInput) {
      el.searchInput.addEventListener('input', (e) => {
        state.searchQuery = e.target.value;
        if (el.searchClearBtn) {
          el.searchClearBtn.style.display = state.searchQuery ? 'block' : 'none';
        }
        // If user searches while not in materials or labs, switch to materials to see results
        if (state.searchQuery && state.activeTab === 'overview') {
          switchTab('materials');
        }
        renderMaterials();
      });
    }

    if (el.searchClearBtn) {
      el.searchClearBtn.addEventListener('click', () => {
        state.searchQuery = '';
        if (el.searchInput) el.searchInput.value = '';
        el.searchClearBtn.style.display = 'none';
        renderMaterials();
      });
    }

    // Mobile menu
    if (el.mobileMenuBtn && el.sidebar) {
      el.mobileMenuBtn.addEventListener('click', () => {
        el.sidebar.classList.toggle('open');
      });
    }

    // Modal close
    if (el.modalCloseBtn) el.modalCloseBtn.addEventListener('click', closePdfViewer);
    if (el.pdfModal) {
      el.pdfModal.addEventListener('click', (e) => {
        if (e.target === el.pdfModal) closePdfViewer();
      });
    }

    // Keyboard shortcuts
    window.addEventListener('keydown', (e) => {
      if (e.key === 'Escape' && el.pdfModal && el.pdfModal.classList.contains('active')) {
        closePdfViewer();
      }
      if (e.key === '/' && document.activeElement !== el.searchInput) {
        e.preventDefault();
        if (el.searchInput) el.searchInput.focus();
      }
    });
  }

  // Utilities
  function escapeHtml(str) {
    if (!str) return '';
    return String(str)
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;')
      .replace(/'/g, '&#039;');
  }

  function escapeAttr(str) {
    if (!str) return '';
    return String(str).replace(/"/g, '&quot;').replace(/'/g, '&#39;');
  }

  // Global API
  window.CGHub = {
    openPdfViewer,
    closePdfViewer,
    switchTab
  };

  // Run on DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
