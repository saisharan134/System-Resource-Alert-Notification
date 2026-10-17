# Test Cases
Project: System Resource Alert and Notification System

| ID | Test | Expected Result |
|---|---|---|
| T1 | Run integrated program | CPU, memory and disk readings displayed |
| T2 | CPU reaches threshold | CPU alert displayed and logged |
| T3 | Memory reaches threshold | Memory alert displayed and logged |
| T4 | Disk reaches threshold | Disk alert displayed and logged |
| T5 | Change config threshold | New threshold loaded |
| T6 | Run continuous mode | Repeated monitoring |
| T7 | Stop with Ctrl+C | Monitoring terminates |

Testing note:
Normal resource monitoring and configuration loading have been demonstrated.
Record the actual result of each additional test before claiming it passed.
