# W1 WPE Soccer

A C++ console program that collects a soccer player’s weekly training and sleep data, evaluates performance level, and displays structured reports.

**Week 8 update:** added a simple `TrainingLog` class to organize setup and menu flow.

## What it does
- Collects player name, age, training sessions (1–5), hours per session, and average sleep
- Validates all numeric input
- Calculates total/average training and a readiness score
- Assigns a level: **Amateur / Semi-Pro / Pro**
- Menu options:
  1. Level evaluation (also writes `report.txt`)
  2. Training plan
  3. Recovery advice

## Key features
- Colored console output (Windows)
- Clear menu-based navigation
- File output: `report.txt` (Week 8 report)

## Structure
- `PlayerWeek` struct stores weekly data
- `PlayerLevel` enum defines level
- `TrainingLog` class manages data collection and menu

## How to run
Build and run in Visual Studio (or any C++ compiler), enter data, then use the menu.
