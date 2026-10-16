# awesome-FOC-motor-control · A Path-Based Learning List for FOC Motor Control

> **简体中文版（full version）请见 [README.md](README.md)** · This is a condensed English overview written for an international audience.

![License](https://img.shields.io/badge/license-MIT-green) ![Stars](https://img.shields.io/github/stars/liujycode/awesome-FOC-motor-control?style=flat&logo=github) ![Last Commit](https://img.shields.io/github/last-commit/liujycode/awesome-FOC-motor-control/main?logo=github) ![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen)

**A curated, path-based learning repository for FOC / PMSM / BLDC motor control — from physics intuition to production delivery. There is no shortage of motor-control materials; what's missing is the right order.**

This is **not** a link dump — it's a **seven-layer skill tree**. Motor-control knowledge is strictly layered: every upper-layer skill assumes the ones below it are already in your hands. If you climb before the lower levels are built, you won't just learn slowly — you'll get stuck at a fixed point, and you'll blame the algorithm, the parameters, or the hardware when the real problem is the missing foundation.

## The Seven-Layer Skill Tree

```
L6  Production delivery   Thermal / EMC / protection / production consistency / cost
L5  Sensorless observers  Sliding-mode / Luenberger / EKF, blind start + handover
L4  Speed / position loops  Outer-loop PI, nested bandwidths, inertia matching, feedforward
L3  FOC current loop      Clarke/Park, SVPWM, current-loop PI, timing budget
L2  Measurement           Oscilloscope triggering, current sensing, encoder alignment
L1  Power stage           Three-phase bridge, MOSFET on/dead-time, PWM, six-step commutation
L0  Physics intuition     Magnetic fields, back-EMF, KV/Kt, commutation
```

**Quick start**: a dev board with three-phase drive (e.g. B-G431B-ESC1 or a SimpleFOC board) + a brushless motor + an oscilloscope. For the first three months: get the motor spinning with six-step commutation, get the waveforms right on the scope, and hand-write a FOC current loop.

## What's Inside

| Category | Highlights |
|---|---|
| **Tutorials, papers & videos** (资料均为**中文**或带**中文**字幕) | Microchip "DC quantities" intuition, TI Precision Labs BLDC course, TI E2E BLDC FAQ, Wescott *PID Without a PhD*, the Bing Driving 60-lecture series (**中文**), vendor app notes (TI/ST/Microchip/NXP/Infineon) |
| **Open-source firmware & projects** | VESC (GPLv3), moteus (Apache-2.0), ODrive (frozen classic), SimpleFOC (MIT), AM32, Tinymovr, bldc_foc — with how-to-read-source notes |
| **Debugging & measurement experience** (mostly triggered from **中文** posts) | 9 field-tested methodologies: learning to trigger before pushing buttons, sampling current in the zero vector, encoder zero alignment, why a blown MOSFET is a result not a cause, timing budgets, inertia ratios, bus pump-up, production consistency, and two physical limits (zero-speed sensorless & simulation) |
| **Interview self-check** | [FOC Interview: 25 High-Frequency Questions](interview/FOC面试高频25问_自测清单.pdf) — 25 questions across L0–L6, each with a level tag and answer skeleton (**中文** PDF + MD **中文**), plus a [checklist card set](checklists/), storage-friendly images |
| **Production-grade C templates** | [tools/](tools/README.md): PI controller (anti-windup, ISR-safe), lock-free ring buffer, and SVPWM — 7-segment sector method with limiting, all 无动态内存/无递归 |

**中文** marks content that is primarily in Chinese; for those, English readers may rely on code, waveforms, and official vendor links. The full list, all layer annotations, and the complete debugging write-ups live in the **[中文 README](README.md)** and under [resources/](resources/).

## How to Use This List

1. **What to learn first** — climb the tree from L0 up. Confirm L0 physics intuition, L1 power stage, and L2 measurement by their pass criteria before going on. The first three layers are the foundation of everything else.
2. **What to build** — at each new layer, do that layer's acceptance action before moving on (e.g. L2: capture PWM / phase current / encoder waveforms on your own, without a library). Whenever something can be **hand-written with the library off** (transforms, SVPWM, current loop), write it by hand.
3. **Where to look when stuck** — return to that layer's deep material: the article index, the per-layer [resources/](resources/) documents, and the [debugging & measurement notes](resources/调试与测量经验.md) for the 9 methodologies.

One line: **don't force higher layers before the foundations are in; verify by doing, not by collecting.**

## Contributing

We accept resources that are real primary sources and methodology proven in practice — no marketing fluff. Please read **[CONTRIBUTING.md](CONTRIBUTING.md)** (收录标准、提交格式、审查流程) before submitting. Issues welcome via the [resource recommendation](.github/ISSUE_TEMPLATE/资源推荐.md) and [broken link](.github/ISSUE_TEMPLATE/链接失效.md) templates.

## Recent Updates

- **2026-10-16**: Added TOC, inclusion criteria, and "how to use this list"; added CONTRIBUTING.md and issue / PR templates; added this English overview.

## License

[MIT](LICENSE). External resources linked (or embedded) remain copyright of their respective owners.

---

Maintained by the WeChat official account **「电机控制深水区」** (Motor Control Deep Water) — a technical account focused on embedded motor control / FOC / power electronics, publishing material that always comes with waveforms, real parameters, and hard-won pitfalls.
