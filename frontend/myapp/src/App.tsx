import React, { useEffect, useMemo, useState } from "react";
import {
  ResponsiveContainer,
  LineChart,
  Line,
  CartesianGrid,
  XAxis,
  YAxis,
  Tooltip,
  Legend,
  BarChart,
  Bar,
} from "recharts";

type Scenario = "randomized" | "worstCase";
type Attribute = "Price" | "Beds" | "Baths" | "House Size";

type BenchmarkRow = {
  size: number;
  merge: number;
  quick: number;
};

type PropertyRow = {
  price: number;
  beds: number;
  baths: number;
  houseSize: number;
  city: string;
  state: string;
};

type BenchmarkData = Record<Attribute, Record<Scenario, BenchmarkRow[]>>;

const benchmarkData: BenchmarkData = {
  Price: {
    randomized: [
      { size: 10000, merge: 12, quick: 9 },
      { size: 50000, merge: 65, quick: 49 },
      { size: 100000, merge: 139, quick: 108 },
      { size: 250000, merge: 381, quick: 344 },
      { size: 500000, merge: 812, quick: 790 },
      { size: 1000000, merge: 1710, quick: 1875 },
    ],
    worstCase: [
      { size: 10000, merge: 10, quick: 54 },
      { size: 50000, merge: 60, quick: 1408 },
      { size: 100000, merge: 127, quick: 5920 },
    ],
  },
  Beds: {
    randomized: [
      { size: 10000, merge: 9, quick: 8 },
      { size: 50000, merge: 52, quick: 43 },
      { size: 100000, merge: 110, quick: 96 },
      { size: 250000, merge: 305, quick: 280 },
      { size: 500000, merge: 652, quick: 691 },
      { size: 1000000, merge: 1418, quick: 1594 },
    ],
    worstCase: [
      { size: 10000, merge: 8, quick: 44 },
      { size: 50000, merge: 47, quick: 1110 },
      { size: 100000, merge: 99, quick: 4388 },
    ],
  },
  Baths: {
    randomized: [
      { size: 10000, merge: 10, quick: 8 },
      { size: 50000, merge: 56, quick: 45 },
      { size: 100000, merge: 119, quick: 97 },
      { size: 250000, merge: 327, quick: 296 },
      { size: 500000, merge: 701, quick: 718 },
      { size: 1000000, merge: 1508, quick: 1623 },
    ],
    worstCase: [
      { size: 10000, merge: 8, quick: 49 },
      { size: 50000, merge: 50, quick: 1267 },
      { size: 100000, merge: 104, quick: 5112 },
    ],
  },
  "House Size": {
    randomized: [
      { size: 10000, merge: 13, quick: 10 },
      { size: 50000, merge: 68, quick: 53 },
      { size: 100000, merge: 145, quick: 113 },
      { size: 250000, merge: 389, quick: 351 },
      { size: 500000, merge: 830, quick: 808 },
      { size: 1000000, merge: 1744, quick: 1901 },
    ],
    worstCase: [
      { size: 10000, merge: 11, quick: 58 },
      { size: 50000, merge: 63, quick: 1462 },
      { size: 100000, merge: 131, quick: 6074 },
    ],
  },
};

const propertyPreview: PropertyRow[] = [
  { price: 129000, beds: 2, baths: 1, houseSize: 960, city: "Ocala", state: "FL" },
  { price: 215000, beds: 3, baths: 2, houseSize: 1440, city: "Tampa", state: "FL" },
  { price: 329900, beds: 4, baths: 3, houseSize: 2010, city: "Austin", state: "TX" },
  { price: 475000, beds: 4, baths: 3, houseSize: 2460, city: "Phoenix", state: "AZ" },
  { price: 710000, beds: 5, baths: 4, houseSize: 3205, city: "San Diego", state: "CA" },
];

function formatNumber(value: number): string {
  return new Intl.NumberFormat("en-US").format(value);
}

function formatMs(value: number): string {
  return `${formatNumber(value)} ms`;
}

function computeStats(rows: BenchmarkRow[]) {
  const latest = rows[rows.length - 1];
  const mergeAvg = rows.reduce((sum, row) => sum + row.merge, 0) / rows.length;
  const quickAvg = rows.reduce((sum, row) => sum + row.quick, 0) / rows.length;
  const fastestAtLargest = latest.merge < latest.quick ? "Merge Sort" : "Quick Sort";
  const speedup =
    Math.max(latest.merge, latest.quick) /
    Math.max(1, Math.min(latest.merge, latest.quick));

  return {
    latest,
    mergeAvg,
    quickAvg,
    fastestAtLargest,
    speedup,
  };
}

function cardStyle(accent: string): React.CSSProperties {
  return {
    background: "#ffffff",
    borderRadius: 20,
    padding: 20,
    boxShadow: "0 10px 30px rgba(15, 23, 42, 0.08)",
    borderTop: `4px solid ${accent}`,
  };
}

const sectionTitleStyle: React.CSSProperties = {
  fontSize: 20,
  fontWeight: 700,
  margin: 0,
  color: "#0f172a",
};

export default function App() {
  const [attribute, setAttribute] = useState<Attribute>("Price");
  const [scenario, setScenario] = useState<Scenario>("randomized");

  const [data, setData] = useState<BenchmarkData & { preview?: PropertyRow[] } | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    async function loadData() {
      try {
        const response = await fetch("/data/benchmark.json");

        if (!response.ok) {
          throw new Error("Failed to load benchmark.json");
        }

        const json = await response.json();
        setData(json);
      } catch (err) {
        setError(err instanceof Error ? err.message : "Unknown error");
      } finally {
        setLoading(false);
      }
    }

    loadData();
  }, []);

  const rows = data ? data[attribute][scenario] : benchmarkData[attribute][scenario];
  const previewRows = data?.preview ?? propertyPreview;

  const stats = useMemo(() => computeStats(rows), [rows]);

  const comparisonRows = useMemo(
    () =>
      rows.map((row) => ({
        ...row,
        winner:
          row.merge < row.quick
            ? "Merge Sort"
            : row.quick < row.merge
              ? "Quick Sort"
              : "Tie",
        difference: Math.abs(row.merge - row.quick),
      })),
    [rows]
  );

  if (loading) {
    return <div style={{ padding: 20 }}>Loading benchmark data...</div>;
  }

  if (error) {
    return (
      <div style={{ padding: 20, color: "red" }}>
        Error loading benchmark data: {error}
      </div>
    );
  }

  return (
    <div
      style={{
        minHeight: "100vh",
        background: "linear-gradient(180deg, #f8fafc 0%, #eef2ff 100%)",
        padding: 24,
        fontFamily:
          'Inter, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif',
        color: "#0f172a",
      }}
    >
      <div style={{ maxWidth: 1280, margin: "0 auto" }}>
        <div
          style={{
            background: "#ffffff",
            borderRadius: 24,
            padding: 24,
            boxShadow: "0 14px 40px rgba(15, 23, 42, 0.08)",
            marginBottom: 24,
          }}
        >
          <div
            style={{
              display: "flex",
              justifyContent: "space-between",
              alignItems: "flex-end",
              gap: 20,
              flexWrap: "wrap",
            }}
          >
            <div style={{ maxWidth: 760 }}>
              <div
                style={{
                  display: "inline-block",
                  padding: "6px 12px",
                  borderRadius: 999,
                  background: "#e0e7ff",
                  color: "#3730a3",
                  fontWeight: 700,
                  fontSize: 12,
                  letterSpacing: 0.3,
                  marginBottom: 12,
                }}
              >
                C++ Algorithm Analysis
              </div>
              <h1 style={{ fontSize: 36, color: "#2563eb", margin: "0 0 10px 0" }}>
                MergeSort vs QuickSort Dashboard
              </h1>
              <p style={{ margin: 0, color: "#475569", lineHeight: 1.6 }}>
                Vite React app made for visualizing real-estate sorting
                benchmarks. Compare runtime growth, inspect worst-case behavior, and
                present algorithm tradeoffs clearly.
              </p>
            </div>

            <div
              style={{
                display: "grid",
                gridTemplateColumns: "repeat(auto-fit, minmax(220px, 1fr))",
                gap: 16,
                minWidth: 320,
              }}
            >
              <div>
                <label
                  htmlFor="attribute-select"
                  style={{ display: "block", fontSize: 14, fontWeight: 600, marginBottom: 8 }}
                >
                  Attribute
                </label>
                <select
                  id="attribute-select"
                  value={attribute}
                  onChange={(e) => setAttribute(e.target.value as Attribute)}
                  style={{
                    width: "100%",
                    padding: "12px 14px",
                    borderRadius: 12,
                    border: "1px solid #cbd5e1",
                    fontSize: 15,
                    background: "#fff",
                  }}
                >
                  <option value="Price">Price</option>
                  <option value="Beds">Beds</option>
                  <option value="Baths">Baths</option>
                  <option value="House Size">House Size</option>
                </select>
              </div>

              <div>
                <div style={{ fontSize: 14, fontWeight: 600, marginBottom: 8 }}>Scenario</div>
                <div
                  style={{
                    display: "grid",
                    gridTemplateColumns: "1fr 1fr",
                    background: "#e2e8f0",
                    borderRadius: 14,
                    padding: 4,
                    gap: 4,
                  }}
                >
                  <button
                    onClick={() => setScenario("randomized")}
                    style={{
                      padding: "10px 12px",
                      borderRadius: 10,
                      border: "none",
                      cursor: "pointer",
                      fontWeight: 700,
                      background: scenario === "randomized" ? "#ffffff" : "transparent",
                      color: "#0f172a",
                    }}
                  >
                    Randomized
                  </button>
                  <button
                    onClick={() => setScenario("worstCase")}
                    style={{
                      padding: "10px 12px",
                      borderRadius: 10,
                      border: "none",
                      cursor: "pointer",
                      fontWeight: 700,
                      background: scenario === "worstCase" ? "#ffffff" : "transparent",
                      color: "#0f172a",
                    }}
                  >
                    Worst Case
                  </button>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div
          style={{
            display: "grid",
            gridTemplateColumns: "repeat(auto-fit, minmax(240px, 1fr))",
            gap: 16,
            marginBottom: 24,
          }}
        >
          <div style={cardStyle("#2563eb")}>
            <div style={{ color: "#64748b", fontSize: 14, marginBottom: 8 }}>Dataset scale</div>
            <div style={{ fontSize: 18, fontWeight: 700, marginBottom: 10 }}>Largest sample</div>
            <div style={{ fontSize: 34, fontWeight: 800 }}>{formatNumber(stats.latest.size)}</div>
            <div style={{ marginTop: 8, color: "#64748b", fontSize: 14 }}>
              Records used in current view
            </div>
          </div>

          <div style={cardStyle("#16a34a")}>
            <div style={{ color: "#64748b", fontSize: 14, marginBottom: 8 }}>Average runtime</div>
            <div style={{ fontSize: 18, fontWeight: 700, marginBottom: 10 }}>Merge Sort</div>
            <div style={{ fontSize: 34, fontWeight: 800 }}>{formatMs(Math.round(stats.mergeAvg))}</div>
            <div style={{ marginTop: 8, color: "#64748b", fontSize: 14 }}>
              Across visible benchmark samples
            </div>
          </div>

          <div style={cardStyle("#f59e0b")}>
            <div style={{ color: "#64748b", fontSize: 14, marginBottom: 8 }}>Average runtime</div>
            <div style={{ fontSize: 18, fontWeight: 700, marginBottom: 10 }}>Quick Sort</div>
            <div style={{ fontSize: 34, fontWeight: 800 }}>{formatMs(Math.round(stats.quickAvg))}</div>
            <div style={{ marginTop: 8, color: "#64748b", fontSize: 14 }}>
              Across visible benchmark samples
            </div>
          </div>

          <div style={cardStyle("#8b5cf6")}>
            <div style={{ color: "#64748b", fontSize: 14, marginBottom: 8 }}>
              Largest-sample winner
            </div>
            <div style={{ fontSize: 18, fontWeight: 700, marginBottom: 10 }}>Best performer</div>
            <div style={{ fontSize: 30, fontWeight: 800 }}>{stats.fastestAtLargest}</div>
            <div style={{ marginTop: 8, color: "#64748b", fontSize: 14 }}>
              {stats.speedup.toFixed(2)}× relative speed advantage
            </div>
          </div>
        </div>

        <div
          style={{
            display: "grid",
            gridTemplateColumns: "minmax(0, 2fr) minmax(280px, 1fr)",
            gap: 24,
            marginBottom: 24,
          }}
        >
          <div
            style={{
              background: "#ffffff",
              borderRadius: 20,
              padding: 20,
              boxShadow: "0 10px 30px rgba(15, 23, 42, 0.08)",
              minWidth: 0,
            }}
          >
            <h2 style={sectionTitleStyle}>Runtime growth by dataset size</h2>
            <div style={{ color: "#64748b", marginTop: 8, marginBottom: 18, lineHeight: 1.5 }}>
              Current attribute: <strong>{attribute}</strong> · Scenario: <strong>
                {scenario === "randomized" ? "Randomized input" : "Pre-sorted worst case"}
              </strong>
            </div>
            <div style={{ width: "100%", height: 360 }}>
              <ResponsiveContainer>
                <LineChart data={rows} margin={{ top: 10, right: 16, left: 8, bottom: 8 }}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="size" tickFormatter={(value) => formatNumber(Number(value))} />
                  <YAxis tickFormatter={(value) => `${value} ms`} />
                  <Tooltip
                    formatter={(value: number | string) => [`${value} ms`, "Runtime"]}
                    labelFormatter={(label) => `Dataset size: ${formatNumber(Number(label))}`}
                  />
                  <Legend />
                  <Line
                    type="monotone"
                    dataKey="merge"
                    name="Merge Sort"
                    stroke="#2563eb"
                    strokeWidth={3}
                    dot={{ r: 4 }}
                  />
                  <Line
                    type="monotone"
                    dataKey="quick"
                    name="Quick Sort"
                    stroke="#f97316"
                    strokeWidth={3}
                    dot={{ r: 4 }}
                  />
                </LineChart>
              </ResponsiveContainer>
            </div>
          </div>

          <div
            style={{
              background: "#ffffff",
              borderRadius: 20,
              padding: 20,
              boxShadow: "0 10px 30px rgba(15, 23, 42, 0.08)",
            }}
          >
            <h2 style={sectionTitleStyle}>Project summary</h2>
            <p style={{ color: "#64748b", marginTop: 8, marginBottom: 20 }}>
              Derived from our C++ project design
            </p>

            <div style={{ display: "grid", gap: 18, color: "#475569", lineHeight: 1.6 }}>
              <div>
                <div style={{ fontWeight: 700, color: "#0f172a" }}>Dataset</div>
                <div>
                  USA real-estate listings with millions of records, sortable by price,
                  beds, baths, or house size.
                </div>
              </div>
              <div>
                <div style={{ fontWeight: 700, color: "#0f172a" }}>Algorithms</div>
                <div>
                  Recursive Merge Sort and iterative Quick Sort implemented from scratch in raw C++.
                </div>
              </div>
              <div>
                <div style={{ fontWeight: 700, color: "#0f172a" }}>Benchmark scenarios</div>
                <div>
                  Randomized full-data runs and pre-sorted worst-case trials that expose Quick Sort degradation.
                </div>
              </div>
              <div>
                <div style={{ fontWeight: 700, color: "#0f172a" }}>Measurement</div>
                <div>Execution time captured in milliseconds.</div>
              </div>
            </div>
          </div>
        </div>

        <div
          style={{
            display: "grid",
            gridTemplateColumns: "repeat(2, minmax(0, 1fr))",
            gap: 24,
            marginBottom: 24,
          }}
        >
          <div
            style={{
              background: "#ffffff",
              borderRadius: 20,
              padding: 20,
              boxShadow: "0 10px 30px rgba(15, 23, 42, 0.08)",
              minWidth: 0,
            }}
          >
            <h2 style={sectionTitleStyle}>Per-sample comparison</h2>
            <p style={{ color: "#64748b", marginTop: 8, marginBottom: 18 }}>
              Lower bars indicate faster execution
            </p>
            <div style={{ width: "100%", height: 320 }}>
              <ResponsiveContainer>
                <BarChart data={comparisonRows} margin={{ top: 10, right: 16, left: 8, bottom: 8 }}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="size" tickFormatter={(value) => formatNumber(Number(value))} />
                  <YAxis tickFormatter={(value) => `${value} ms`} />
                  <Tooltip
                    formatter={(value: number | string) => [`${value} ms`, "Runtime"]}
                    labelFormatter={(label) => `Dataset size: ${formatNumber(Number(label))}`}
                  />
                  <Legend />
                  <Bar dataKey="merge" name="Merge Sort" fill="#2563eb" radius={[8, 8, 0, 0]} />
                  <Bar dataKey="quick" name="Quick Sort" fill="#f97316" radius={[8, 8, 0, 0]} />
                </BarChart>
              </ResponsiveContainer>
            </div>
          </div>

          <div
            style={{
              background: "#ffffff",
              borderRadius: 20,
              padding: 20,
              boxShadow: "0 10px 30px rgba(15, 23, 42, 0.08)",
            }}
          >
            <h2 style={sectionTitleStyle}>Sorted output </h2>

            <div style={{ border: "1px solid #e2e8f0", borderRadius: 16, overflow: "hidden" }}>
              <div
                style={{
                  display: "grid",
                  gridTemplateColumns: "repeat(6, 1fr)",
                  gap: 12,
                  background: "#f8fafc",
                  padding: 14,
                  fontSize: 12,
                  fontWeight: 800,
                  textTransform: "uppercase",
                  color: "#64748b",
                  letterSpacing: 0.5,
                }}
              >
                <div>Price</div>
                <div>Beds</div>
                <div>Baths</div>
                <div>Sqft</div>
                <div>City</div>
                <div>State</div>
              </div>

              {previewRows.map((row, index) => (
                <div
                  key={`${row.city}-${index}`}
                  style={{
                    display: "grid",
                    gridTemplateColumns: "repeat(6, 1fr)",
                    gap: 12,
                    padding: 14,
                    borderTop: index === 0 ? "none" : "1px solid #e2e8f0",
                    fontSize: 14,
                    color: "#334155",
                  }}
                >
                  <div>${formatNumber(row.price)}</div>
                  <div>{row.beds}</div>
                  <div>{row.baths}</div>
                  <div>{formatNumber(row.houseSize)}</div>
                  <div>{row.city}</div>
                  <div>{row.state}</div>
                </div>
              ))}
            </div>
          </div>
        </div>

        <div
          style={{
            background: "#ffffff",
            borderRadius: 20,
            padding: 20,
            boxShadow: "0 10px 30px rgba(15, 23, 42, 0.08)",
          }}
        >
          <h2 style={sectionTitleStyle}>Benchmark table</h2>

          <div style={{ overflowX: "auto", border: "1px solid #e2e8f0", borderRadius: 16 }}>
            <table style={{ width: "100%", borderCollapse: "collapse", minWidth: 700 }}>
              <thead style={{ background: "#f8fafc", color: "#64748b" }}>
                <tr>
                  <th style={{ textAlign: "left", padding: 14 }}>Dataset size</th>
                  <th style={{ textAlign: "left", padding: 14 }}>Merge Sort</th>
                  <th style={{ textAlign: "left", padding: 14 }}>Quick Sort</th>
                  <th style={{ textAlign: "left", padding: 14 }}>Winner</th>
                  <th style={{ textAlign: "left", padding: 14 }}>Difference</th>
                </tr>
              </thead>
              <tbody>
                {comparisonRows.map((row) => (
                  <tr key={row.size} style={{ borderTop: "1px solid #e2e8f0", color: "#334155" }}>
                    <td style={{ padding: 14 }}>{formatNumber(row.size)}</td>
                    <td style={{ padding: 14 }}>{formatMs(row.merge)}</td>
                    <td style={{ padding: 14 }}>{formatMs(row.quick)}</td>
                    <td style={{ padding: 14 }}>{row.winner}</td>
                    <td style={{ padding: 14 }}>{formatMs(row.difference)}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
  );
}
