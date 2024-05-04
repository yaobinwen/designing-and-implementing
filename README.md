# Designing and Implementing a Software System

## Overview

This repository has the notes of my learning and thinking in how to **design and implement a software system in a systematic method**. I focus on software systems because I am a software developer, not having too much experience in dealing with hardware. However, because software runs on top of hardware, hardware will inevitably get involved into the discussion to some extent.

Why is this topic important, at least to me? My answer is: because I want to reproduce success in every piece of software development task. Based on my personal experience, I have realized that, without a systematic method or process, I oftentimes solve the tasks at hand on an ad hoc basis: I may solve it for this particular task well enough, but I'm not sure if I can solve the next task the same well enough or even better.

## Key ideas

- Model the target software system as a state machine. Related:
  - "Write-ahead logs (WAL)" or similar technologies might be important to implement correct state transition.
  - Exception safety.
- Design the architecture.
  - See [Martin Fowler: Software Architecture Guide](https://martinfowler.com/architecture/): Identify the "important stuff".
- Define the scope of the target software system in order to distinguish the errors that can be handled and the errors that can't be handled.
- Report and handle errors correctly. See the sub-folder [Error-Handling](./Error-Handling).
- Information security.
- Performance (or the so-called "non-functional requirements").
- Operation: We can't simply look at the software system itself. We must consider its upstream (i.e., how to get the needed material in order to reconstruct the software system automatically) and downstream (i.e., once the software system is upgraded, how to deploy it to the field automatically).

## State machine

[Every Computer System is a State Machine](https://blog.the-pans.com/state-machine/) (or see [the discussion on HN](https://news.ycombinator.com/item?id=23425508)).

TODOs:
- [ASC: Automatically scalable computation](https://collaborate.princeton.edu/en/publications/asc-automatically-scalable-computation)
  - Video: [Margo Seltzer - Automatically Scalable Computation - Code Mesh 2017](https://www.youtube.com/watch?v=MHZDXC4zJ0c)
- [The TLA+ Video Course](https://lamport.azurewebsites.net/video/videos.html)
- [Learn TLA+](https://learntla.com/)
- [Chomsky hierarchy](https://en.wikipedia.org/wiki/Chomsky_hierarchy)
  - [Pushdown automaton](https://en.wikipedia.org/wiki/Pushdown_automaton)
- [Computer Organization and Design Fundamentals by David Tarnoff](http://faculty.etsu.edu/tarnoff/ntes2150/statemac/statemac.htm)
