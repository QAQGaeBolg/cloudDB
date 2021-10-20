import { NodeAny } from "./nodes/NodeAny"
import { NodeNumber } from "./nodes/NodeNumber"
import { TreeNode } from "./TreeNode"

export class BTree{
    private root: TreeNode
    private stage: number
    private leastNodeNumber: number
    private mostNodeNumber: number

    constructor(stage: number){
        this.root = null
        this.stage = stage
        this.leastNodeNumber = Math.ceil(this.stage / 2)
        this.mostNodeNumber = this.stage 
    }

    private isBalance(node: TreeNode): boolean {
        return this.leastNodeNumber <= node.list.length && node.list.length <= this.mostNodeNumber
    }

    private find(node: TreeNode, value: NodeAny): boolean {
        if (node === null) return false
        for (let i = 1; i < node.list.length; i++) {
            let item = node.list[i]
            if (TreeNode.equal(value, item)) {
                return true;
            } else if (TreeNode.less(value, item)) {
                return this.find(node.list[i - 1].pointer, value)
            } else if (i == node.list.length - 1) {
                return this.find(item.pointer, value)
            }
        }
    }

    private split(parent: TreeNode, child: TreeNode): void {
        let mid = child.list.length >> 1
        let key = child.list[mid].key        
        if (parent === null) {
            let left = new TreeNode(child)
            let right = new TreeNode(child)
            for (let i = 0; i < mid; i++) {
                left.push(child.list[i])
            }
            left.list[0].key = mid - 1
            right.list.push(new NodeNumber(child.list.length - mid - 1, child.list[mid].pointer))
            for (let i = mid + 1; i < child.list.length; i++) {
                right.list.push(child.list[i])
            }
            child = new TreeNode(parent)
            child.list.push(new NodeNumber(1, left))
            child.list.push(new NodeNumber(key, right))
            return
        } else {
            let node = new TreeNode(parent)
            let nodeItem = child.list[mid]
            node.list.push(new NodeNumber(child.list.length - mid - 1, child.list[mid].pointer))
            for (let i = mid + 1; i < child.list.length; i++) {
                node.list.push(child.list[i])
            }
            child.list.slice(0, mid)
            nodeItem.pointer = node
            parent.list.push(nodeItem)
            let pos = parent.list.length - 1
            while (pos >= 1 && parent.list[pos].less(parent[pos - 1])) {
                let temp = node.list[pos - 1]
                node.list[pos - 1] = node.list[pos]
                node.list[pos] = temp
            }
        }
    }

    private insert(parent: TreeNode, node: TreeNode, value: NodeAny): void {
        if (node === this.root && this.find(node, value)) {
            return
        }
        if(node === null) {
            node = new TreeNode(parent)
            node.list.push(new NodeNumber(1, null))
            node.list.push(value)
            return
        }
        if (this.isBalance(node)) {
            node.list.push(value)
            let pos = node.list.length - 1
            while (pos >= 1 && node.list[pos].less(node.list[pos - 1])) {
                let temp = node.list[pos - 1]
                node.list[pos - 1] = node.list[pos]
                node.list[pos] = temp
            }
            if (!this.isBalance(node)) {
                this.split(parent, node)
            }
            return
        }
        for (let i = 1; i < node.list.length; i++) {
            let item = node.list[i]
            if (value.less(item)) {
                let subNode = node.list[i - 1].pointer
                this.insert(node, subNode, value)
                if (!this.isBalance(node)) {
                    this.split(parent, node)
                }
                return
            }
        }
    }

    private findPre(node: TreeNode, pos: number): TreeNode {
        let subNode = node.list[pos - 1].pointer
        if (subNode === null) {
            return null
        }
        let ans = null
        while (subNode !== null) {
            ans = subNode
            subNode = subNode.list[subNode.list.length - 1].pointer
        }
        return ans
    }

    private findSuc(node: TreeNode, pos: number): TreeNode {
        let subNode = node.list[pos].pointer
        if (subNode === null) {
            return null
        }
        let ans = null
        while (subNode!== null) {
            ans = subNode
            subNode = subNode.list[1].pointer
        }
        return ans
    }

    private findPos(node: TreeNode): number {
        let father = node.father
        for (let i = 1; i < father.list.length; i++) {
            if (node.list[1].less(father.list[i])) {
                return i - 1
            }
        }
        return father.list.length - 1
    }

    private merge(parent: TreeNode, node: TreeNode): void {
        let pos = this.findPos(node)
        let suc = this.findSuc(parent, pos)
        let temp = parent.list[pos]        
        if (suc.list.length == this.stage) {
            let right = temp.pointer
            node.list.push(temp)
            for (let i = 1; i < right.list.length; i++) {
                node.list.push(right.list[i])
            }
            parent.list = parent.list.filter(item => item !== temp)
            temp.pointer = null           
        } else {
            temp.pointer = null
            node.list.push(temp);
            parent.list[pos] = suc.list[1]
            parent.list[pos].pointer = suc
            let remove = suc.list[1]
            suc.list = suc.list.filter(item => item !== remove)            
        }
        if (!this.isBalance(parent)) {
            this.merge(parent.father, parent)
        }
    }

    private delete(parent: TreeNode, node: TreeNode, value: NodeAny): void {
        if (node === null) {
            return
        }
        for(let i = 1; i < node.list.length; i++) {
            if (value.less(node.list[i])) {
                this.delete(node, node.list[i - 1].pointer, value)
                return
            } else if (value.equal(node.list[i])) {
                let now = node
                if(node.list[i].pointer === null) {
                    let nodeItem = node.list[i]
                    node.list = node.list.filter(item => item !== nodeItem)
                } else {
                    let pre = this.findPre(node, i)
                    let temp = pre.list[pre.list.length - 1].key
                    pre.list[pre.list.length - 1].key = node.list[i].key
                    node.list[i].key = temp
                    pre.list.slice(0, pre.list.length - 1)
                    now = pre
                }
                if (!this.isBalance(now)) {
                    this.merge(now.father, now)
                }
                return

            } else if(i == node.list.length - 1) {
                this.delete(node, node.list[i].pointer, value)
                return
            }
        }
    }
}