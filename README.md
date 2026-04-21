# PES-VCS Lab Report

**Name:** Sumukh Kuradi Kalkura  
**SRN:** PES1UG24CS244  
**Repository:** PES1UG24CS244-pes-vcs  
**Platform:** Ubuntu 22.04  

---

## Phase 1 — Object Storage

**Screenshot 1a**  
_Object creation and storage output_
<img width="1108" height="215" alt="Pasted Graphic" src="https://github.com/user-attachments/assets/38b8dfec-08ef-4be0-9ef6-7e0accc55e20" />

**Screenshot 1b**  
_Object verification and test results_
<img width="1039" height="123" alt="Pasted Graphic 1" src="https://github.com/user-attachments/assets/c10b2484-3f96-4989-9529-d76fed0090b4" />

---

## Phase 2 — Tree Objects

**Screenshot 2a**  
_Tree serialization and parsing output_
<img width="920" height="165" alt="PASS tree sertaltzeparse roundtrip" src="https://github.com/user-attachments/assets/58783c4b-f576-49b7-b2a0-ac74aa285009" />

**Screenshot 2b**  
_Deterministic tree structure verification_
<img width="1221" height="127" alt="Pasted Graphic 3" src="https://github.com/user-attachments/assets/673c8db1-93a5-45f0-b849-078b5674af33" />

---

## Phase 3 — Index / Staging Area

**Screenshot 3a**  
_Using `pes add` and staging files_
<img width="1075" height="648" alt="Stseenfitel tx" src="https://github.com/user-attachments/assets/a57908d2-02f0-409e-8a51-3491801dc775" />

**Screenshot 3b**  
_Verification of index functionality_
<img width="975" height="110" alt="Pasted Graphic 5" src="https://github.com/user-attachments/assets/2bd61db2-69f4-418d-8932-bfc2a27a1050" />

---

## Phase 4 — Commits and History

**Screenshot 4a**  
`./pes log` showing multiple commits and history traversal  
<img width="896" height="366" alt="Comnitted" src="https://github.com/user-attachments/assets/a7c78910-0bbb-474f-aabf-db0084ebb055" />

**Screenshot 4b**  
`find .pes -type f | sort` showing object store growth after commits  
<img width="1077" height="663" alt="Pasted Graphic 8" src="https://github.com/user-attachments/assets/ce6f9430-41e3-4930-9407-480a4159c1a1" />

**Screenshot 4c**  
`cat .pes/refs/heads/main` and `cat .pes/HEAD` showing reference chain  
<img width="1207" height="96" alt="Pasted Graphic 9" src="https://github.com/user-attachments/assets/b1708238-9315-4f75-8c71-afe73aa0effc" />

---

## Final Test

### Final Test — Part 1  
_Initialization and file staging_
<img width="1033" height="585" alt="Pasted Graphic 10" src="https://github.com/user-attachments/assets/89fb0cb3-d378-4d7d-a550-d6d963a42c75" />

### Final Test — Part 2  
_Commit creation and object storage_
<img width="1058" height="664" alt="Comictes alelfese4s1a  Initial comit" src="https://github.com/user-attachments/assets/ebbe4363-aa8e-4292-8d6d-0efe7652d062" />

### Final Test — Part 3  
_Log traversal and verification_
<img width="1186" height="702" alt="reti retshastsnatn" src="https://github.com/user-attachments/assets/55eba06f-c482-4696-8caf-9fdcbc1d10ad" />

---

## Analysis Questions

### Q5.1 — Implementing pes checkout <branch>

To implement pes checkout <branch>, the repository must update .pes/HEAD and the branch reference under .pes/refs/heads/<branch>. If the checkout is to a normal branch, HEAD should continue pointing to the branch name rather than a raw commit hash. The working directory must then be rewritten so it matches the tree of the target commit: files that no longer exist on the target branch must be removed, files that differ must be replaced, and new files and directories from the target branch must be created.

This operation is complex because it must protect user data. Checkout cannot blindly overwrite tracked files if the user has uncommitted local changes. It also has to handle nested directories, deletions, file modes, and untracked files that might be clobbered by the switch.

---

### Q5.2 — Detecting dirty working directory conflicts

A dirty working directory conflict can be detected by comparing the current working directory, the index, and the target branch’s tree. For each tracked path, compare the file in the working directory against the index metadata first. If size or modification time changed, the file may be dirty. For a stronger check, hash the working file and compare it against the blob hash stored in the index. Then compare the target branch’s tree entry for that path with the current staged version.

If a tracked file is modified locally and the target branch has a different version of that same file, checkout should refuse. In other words, checkout must stop whenever switching branches would overwrite uncommitted changes.

---

### Q5.3 — Detached HEAD

Detached HEAD means HEAD stores a commit hash directly instead of a branch reference. Commits made in this state still create valid new commit objects, but no branch pointer is advanced to remember them. The new commits are reachable only through the direct HEAD history until the user switches away.

A user can recover detached-HEAD commits by creating a new branch at the commit hash, or by updating an existing branch reference to that hash. If the hash is still known, the commit history can be preserved without loss.

---

### Q6.1 — Garbage collection and space reclamation

Garbage collection should begin from all branch tips and from HEAD if it points directly to a commit. From those roots, the algorithm recursively traverses commit parents and commit trees, and then each tree’s child blobs and subtrees. Every visited object hash is inserted into a hash set to avoid revisiting the same object multiple times.

After traversal, any object not in the reachable set can be deleted. A hash set is the right data structure because lookups are fast and duplicates are naturally eliminated. With 100,000 commits and 50 branches, the number of objects visited is approximately the number of unique reachable commits plus the trees and blobs they reference. Because branches usually share history, the traversal is closer to the size of the reachable graph than to 50 times the number of commits.

---

### Q6.2 — Why concurrent GC is dangerous

Running garbage collection concurrently with commit creation is dangerous because commit creation is multi-step. A commit may write blobs first, then trees, then the commit object, and finally update the branch ref. If GC runs in the middle, it may see the new blob or tree as unreachable and delete it before the commit object or ref update makes it reachable.

Git avoids this with locking, atomic ref updates, and grace periods before pruning unreachable objects. Recently created or recently referenced objects are not deleted immediately, which prevents GC from racing against in-progress commits.

---
