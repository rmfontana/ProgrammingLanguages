;simplify arbitrary boolean expressions: accepts "and", "or", "not"


(defn simplify-or [l]
  (let [newl (remove false? l)]
  (let [newl2 (nthrest newl 1)]
  (cond
    (some true? newl2) true
    (empty? newl2) false
    (=(count newl2) 1) (last newl2)

    :else newl))))

(defn simplify-and [l]
  (let [newl (remove true? l)]
  (let [newl2 (nthrest newl 1)]
  (cond 
    (some false? newl2) false
    (=(count newl2) 1) (last newl2)
    (some symbol? newl2) newl

    :else true))))

(defn simplify-not [l]
  (let [newl (nthrest l 1)]
  (cond 
  (every? true? newl) false
  (every? false? newl) true )))

(defn simplify3 [l]
    (map (fn [i]
        (if (seq? i) 
          (let [const (first i)] 
            (cond
              (some seq? i) (simplify3 i)
              (= 'or const) (simplify-or i)
			        (= 'and const) (simplify-and i)
			        (= 'not const) (simplify-not i)
              
              :basecase i))
         i)) 
    l ))

(defn simplify2 [l]
(let [const (first l)]
  (cond 
    (some seq? l) (simplify2(simplify3 l)) ;;if there's still more, still do more
    (= 'or const) (simplify-or l) ;;the last one doesn't get reached, so take care of that
    (= 'and const) (simplify-and l)
    (= 'not const) (simplify-not l))))

(defn simplify [l] 
(let [j (simplify2 l)]
  (cond 
    (true? j) true
    (false? j) false

    :else (last(distinct j)))))


(defn bind-values [m l]
  (map (fn [i]
        (if (seq? i)
          (bind-values m i)
          (m i i)))
          l))

(defn evalexp [exp bindings] (simplify (bind-values bindings exp)))
