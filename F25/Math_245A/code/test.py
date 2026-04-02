# save as draw_ot_diracs.py
import numpy as np
import matplotlib.pyplot as plt

# --- Style: enable LaTeX-like math rendering (optional) ---
plt.rcParams.update({
    "text.usetex": True,       # requires LaTeX installed
    "font.family": "serif",
    "font.size": 12,
})

def draw_diracs(ax, xs, weights, y_base=0.0, spike_height=1.0, color='C0', label_prefix='\\delta'):
    """Draw Dirac masses as vertical spikes and labels."""
    xs = np.array(xs)
    weights = np.array(weights)
    # use spike heights scaled by weight (or use constant)
    heights = spike_height * (weights / weights.max())
    (markerline, stemlines, baseline) = ax.stem(xs, heights + y_base, basefmt=" ")
    plt.setp(markerline, marker='o', markersize=6, color=color)
    plt.setp(stemlines, linewidth=1.5, color=color)
    for xi, wi, hi in zip(xs, weights, heights):
        ax.text(xi, hi + y_base + 0.06*spike_height, rf"${wi}\,{label_prefix}_{{{xi}}}$",
                ha='center', va='bottom', fontsize=10)

def draw_coupling(ax, xs, ys, plan, y_src=-0.1, y_tgt=0.6, max_linewidth=8, cmap='viridis'):
    """
    Draw edges from xs (source) to ys (target) with thickness proportional to plan mass.
    plan is a len(xs) x len(ys) matrix of nonnegative masses.
    """
    xs = np.array(xs)
    ys = np.array(ys)
    plan = np.array(plan)
    # normalize for linewidth scaling
    maxmass = plan.max() if plan.max() > 0 else 1.0
    for i, xi in enumerate(xs):
        for j, yj in enumerate(ys):
            mass = plan[i, j]
            if mass <= 0: 
                continue
            lw = max_linewidth * (mass / maxmass)
            # draw a curved line for visual clarity
            midx = (xi + yj) / 2
            # Bezier-ish curve using simple quadratic
            verts_x = [xi, midx, yj]
            verts_y = [y_src, (y_src + y_tgt)/2 + 0.08*abs(yj-xi), y_tgt]
            ax.plot(verts_x, verts_y, linewidth=lw, alpha=0.8, solid_capstyle='round', color='gray')

def example_and_save(filename="ot_diracs_example.pdf"):
    fig, ax = plt.subplots(figsize=(6,3))
    # Example measures:
    xs = [-1.2, 0.0, 1.5]         # support of mu
    a = [0.4, 0.2, 0.4]          # weights for mu
    ys = [-0.8, 0.5, 1.8]         # support of nu
    b = [0.5, 0.25, 0.25]        # weights for nu

    # A few example transport plans (rows sum to a, cols sum to b)
    # 1) Monge-ish map (mass pushed deterministically): put full mass along matching indices
    plan1 = np.zeros((3,3))
    plan1[0,0] = 0.4
    plan1[1,1] = 0.2
    plan1[2,2] = 0.4

    # 2) Split mass: second source splits to two targets
    plan2 = np.array([[0.4, 0.0, 0.0],
                      [0.1, 0.05, 0.05],
                      [0.0, 0.25, 0.15]])

    # 3) Fully-coupled (dense) example
    plan3 = np.array([[0.2, 0.1, 0.1],
                      [0.15, 0.05, 0.0],
                      [0.15, 0.1, 0.15]])

    # Choose which plan to draw:
    plan = plan3

    # Draw sources (bottom line) and targets (top line)
    draw_diracs(ax, xs, a, y_base=-0.08, spike_height=0.5, color='C1', label_prefix='\\delta^{\\mu}')
    draw_diracs(ax, ys, b, y_base=0.88, spike_height=0.5, color='C2', label_prefix='\\delta^{\\nu}')

    draw_coupling(ax, xs, ys, plan, y_src=0.0, y_tgt=0.8, max_linewidth=7)

    # cosmetic
    ax.set_xlim(min(min(xs), min(ys)) - 0.8, max(max(xs), max(ys)) + 0.8)
    ax.set_ylim(-0.3, 1.2)
    ax.axis('off')

    # title and text annotation
    ax.text(0.0, 1.05, r"\textbf{Transport plan (thickness $\propto$ mass)}", ha='center')
    plt.tight_layout()
    fig.savefig(filename, dpi=300)   # PDF/SVG recommended; change extension to .svg or .pdf
    print("Saved:", filename)

if __name__ == "__main__":
    example_and_save("ot_diracs_example.pdf")

